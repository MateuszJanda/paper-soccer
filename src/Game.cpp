#include "Game.hpp"
#include "NCurses.hpp"
#include <iostream>
#include <sstream>

namespace PaperSoccer {

template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};

template <class... Ts>
overloaded(Ts...)->overloaded<Ts...>;

namespace {

    bool isGoalToOwn(MoveStatus status, Goal goal)
    {
        return (status == MoveStatus::TopGoal and goal == Goal::Top) or (status == MoveStatus::BottomGoal and goal == Goal::Bottom);
    }

    bool isGoalToEnemy(MoveStatus status, Goal goal)
    {
        return (status == MoveStatus::TopGoal and goal == Goal::Bottom) or (status == MoveStatus::BottomGoal and goal == Goal::Top);
    }

} // namespace anonymous

Game::Game(INetwork& network, IBoard& board, INCurses& ncurses, IView& view)
    : m_network{network}
    , m_board{board}
    , m_ncurses{ncurses}
    , m_view{view}
{
}

void Game::run()
{
    using namespace std::placeholders;

    m_network.registerHandlers(std::bind(&Game::onKeyboardMouseInput, this),
        [this]() { initNewGame(); },
        [this](NewGameMsg msg) { onNewGame(msg); },
        [this](MoveMsg msg) { onEnemyMove(msg); },
        [this](UndoMoveMsg) { onEnemyUndoMove(); },
        [this](EndTurnMsg) { onEnemyEndTurn(); },
        [this](ReadyForNewGameMsg) { onEnemyReadyForNewGame(); });
    m_network.run();
}

void Game::initNewGame(Goal userGoal)
{
    m_firstTurn = (m_firstTurn == Turn::Enemy) ? Turn::User : Turn::Enemy;
    m_currentTurn = m_firstTurn;
    m_userGoal = userGoal;

    resetSettings();

    Turn turnForEnemy = (m_firstTurn == Turn::Enemy) ? Turn::User : Turn::Enemy;
    Goal enemyGoal = (m_userGoal == Goal::Top) ? Goal::Bottom : Goal::Top;
    m_network.sendNewGame(turnForEnemy, enemyGoal);
}

void Game::onNewGame(NewGameMsg msg)
{
    m_firstTurn = msg.turn;
    m_currentTurn = m_firstTurn;
    m_userGoal = msg.goal;

    resetSettings();
}

void Game::resetSettings()
{
    m_match = MatchStatus::InProgress;
    m_userStatus = MoveStatus::Continue;
    m_enemyStatus = MoveStatus::Continue;

    m_view.clear();
    m_board.reset();

    if (m_currentTurn == Turn::User) {
        m_view.setContinueStatus();
    } else {
        m_view.setEnemyTurnStatus();
    }

    m_view.drawLegend(UNDO_MOVE_KEY, NEW_GAME_KEY, DIR_KEYS);
    m_view.drawScore(m_userScore, m_enemyScore);
    drawBoard();
}

void Game::drawBoard()
{
    if (m_userGoal == Goal::Top) {
        m_view.drawBoard("You", "Enemy", m_dirPath);
    } else {
        m_view.drawBoard("Enemy", "You", m_dirPath);
    }
}

void Game::onKeyboardMouseInput()
{
    for (;;) {
        auto input = m_ncurses.getInput();

        if (not input)
            break;

        std::visit(overloaded{
                       [this](const KeyInput& data) { userKey(data.key); },
                       [this](const MouseInput& data) { userMouse(data.x, data.y); },
                       [this](const EnterInput& data) { userEndTurn(); },
                   },
            *input);
    }
}

void Game::userKey(int key)
{
    if (DIR_KEYS.contains(key) and m_match == MatchStatus::InProgress) {
        Direction dir = DIR_KEYS.at(key);
        userMove(dir);
    } else if (key == UNDO_MOVE_KEY) {
        userUndoMove();
    } else if (key == NEW_GAME_KEY) {
        userRequestNewGame();
    }
}

void Game::userMove(Direction dir)
{
    if (m_currentTurn == Turn::Enemy or m_userStatus != MoveStatus::Continue) {
        return;
    }

    const auto status = m_board.moveBall(dir);

    if (status == MoveStatus::Illegal) {
        return;
    }

    m_userStatus = status;

    if (m_userStatus != MoveStatus::Continue) {
        m_view.setReadyToEndTurnStatus();
    }

    m_dirPath.push_back(dir);
    drawBoard();
    m_network.sendMove(dir);
}

void Game::userUndoMove()
{
    if (m_currentTurn == Turn::Enemy or m_dirPath.empty()) {
        return;
    }

    const auto dir = m_dirPath.back();
    m_dirPath.pop_back();
    const auto reverseDir = reverseDirection(dir);
    m_board.undoBallMove(reverseDir);
    m_userStatus = MoveStatus::Continue;

    m_view.setContinueStatus();
    drawBoard();

    m_network.sendUndoMove();
}

void Game::userRequestNewGame()
{
    if (m_match == MatchStatus::GameEnd) {
        m_match = MatchStatus::ReadyForNew;
        m_network.sendReadyForNewGame();
    } else if (m_match == MatchStatus::EnemyReadyForNew) {
        initNewGame(m_userGoal);
    }
}

void Game::userMouse(int x, int y)
{
    if (m_view.isStatusButton(x, y)) {
        userEndTurn();
    }
    else if (const auto dir = m_view.getMoveDirection(x, y); dir) {
        userMove(*dir);
    }
}

void Game::userEndTurn()
{
    if (m_currentTurn == Turn::Enemy or m_userStatus == MoveStatus::Continue) {
        return;
    }

    if (m_userStatus == MoveStatus::DeadEnd or isGoalToOwn(m_userStatus, m_userGoal)) {
        m_match = MatchStatus::GameEnd;
        m_enemyScore += 1;
        m_view.setLostStatus(m_userScore, m_enemyScore);
    } else if (isGoalToEnemy(m_userStatus, m_userGoal)) {
        m_match = MatchStatus::GameEnd;
        m_userScore += 1;
        m_view.setWinStatus(m_userScore, m_enemyScore);
    } else {
        m_currentTurn = Turn::Enemy;
        m_view.setEnemyTurnStatus();
    }

    m_dirPath.clear();
    drawBoard();

    m_network.sendEndTurn();
}

void Game::onEnemyMove(MoveMsg msg)
{
    if (m_currentTurn == Turn::User) {
        throw std::invalid_argument{"Enemy move in user turn."};
    }

    const auto status = m_board.moveBall(msg.dir);

    if (status == MoveStatus::Illegal) {
        throw std::invalid_argument{"Illegal move."};
    }

    m_enemyStatus = status;
    m_dirPath.push_back(msg.dir);
    drawBoard();
}

void Game::onEnemyUndoMove()
{
    if (m_currentTurn == Turn::User or m_dirPath.empty()) {
        return;
    }

    const auto dir = m_dirPath.back();
    m_dirPath.pop_back();
    const auto reverseDir = reverseDirection(dir);
    m_board.undoBallMove(reverseDir);
    m_enemyStatus = MoveStatus::Continue;

    m_view.setEnemyTurnStatus();
    drawBoard();
}

void Game::onEnemyEndTurn()
{
    if (m_currentTurn == Turn::User) {
        throw std::invalid_argument{"Enemy end turn in user turn."};
    }

    if (m_enemyStatus == MoveStatus::Continue) {
        throw std::invalid_argument{"Enemy end turn but doesn't make all moves."};
    }

    if (isGoalToOwn(m_enemyStatus, m_userGoal)) {
        m_match = MatchStatus::GameEnd;
        m_enemyScore += 1;
        m_view.setLostStatus(m_userScore, m_enemyScore);
    } else if (m_enemyStatus == MoveStatus::DeadEnd or isGoalToEnemy(m_enemyStatus, m_userGoal)) {
        m_match = MatchStatus::GameEnd;
        m_userScore += 1;
        m_view.setWinStatus(m_userScore, m_enemyScore);
    } else {
        m_currentTurn = Turn::User;
        m_userStatus = MoveStatus::Continue;
        m_view.setContinueStatus();
    }

    m_dirPath.clear();
    drawBoard();
}

void Game::onEnemyReadyForNewGame()
{
    if (m_match == MatchStatus::GameEnd) {
        m_match = MatchStatus::EnemyReadyForNew;
    }
}

} // namespace PaperSoccer
