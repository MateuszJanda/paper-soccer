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
        std::bind(&Game::initNewGame, this),
        std::bind(&Game::onNewGame, this, _1),
        std::bind(&Game::onEnemyMove, this, _1),
        std::bind(&Game::onEnemyUndoMove, this, _1),
        std::bind(&Game::onEnemyEndTurn, this, _1),
        std::bind(&Game::onEnemyReadyForNewGame, this, _1));
    m_network.run();
}

void Game::initNewGame()
{
    m_match = MatchStatus::InProgress;
    m_firstTurn = (m_firstTurn == Turn::Enemy) ? Turn::User : Turn::Enemy;
    m_currentTurn = m_firstTurn;
    m_userGoal = Goal::Top;
    m_userStatus = MoveStatus::Continue;
    m_enemyStatus = MoveStatus::Continue;

    m_board.reset();
    m_view.drawBoard();

    if (m_currentTurn == Turn::User) {
        m_view.setContinueStatus();
    } else {
        m_view.setEnemyTurnStatus();
    }

    Turn turnForEnemy = (m_firstTurn == Turn::Enemy) ? Turn::User : Turn::Enemy;
    Goal enemyGoal = (m_userGoal == Goal::Top) ? Goal::Bottom : Goal::Top;
    m_network.sendNewGame(turnForEnemy, enemyGoal);
}

void Game::onNewGame(NewGameMsg msg)
{
    m_match = MatchStatus::InProgress;
    m_firstTurn = msg.turn;
    m_currentTurn = m_firstTurn;
    m_userGoal = msg.goal;
    m_userStatus = MoveStatus::Continue;
    m_enemyStatus = MoveStatus::Continue;

    m_board.reset();
    m_view.drawBoard();

    if (m_currentTurn == Turn::User) {
        m_view.setContinueStatus();
    } else {
        m_view.setEnemyTurnStatus();
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
                       [this](const EnterInput& data) { userEndTurn(); },
                       [this](const MouseInput& data) { },
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
    m_view.drawBoard();

    if (status == MoveStatus::Illegal) {
        return;
    }

    m_userStatus = status;

    if (m_userStatus != MoveStatus::Continue) {
        m_view.setReadyToEndTurnStatus();
    }

    m_dirPath.push_back(dir);
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
    m_view.drawBoard();
    m_network.sendUndoMove();
}

void Game::userRequestNewGame()
{
    if (m_match == MatchStatus::GameEnd) {
        m_match = MatchStatus::ReadyForNew;
        m_network.sendReadyForNewGame();
    } else if (m_match == MatchStatus::EnemyReadyForNew) {
        initNewGame();
    }
}

void Game::userEndTurn()
{
    if (m_currentTurn == Turn::Enemy or m_userStatus == MoveStatus::Continue) {
        return;
    }

    if (m_userStatus == MoveStatus::DeadEnd or isGoalToOwn(m_userStatus, m_userGoal)) {
        m_match = MatchStatus::GameEnd;
        m_view.setLostStatus();
    } else if (isGoalToEnemy(m_userStatus, m_userGoal)) {
        m_match = MatchStatus::GameEnd;
        m_view.setWinStatus();
    } else {
        m_currentTurn = Turn::Enemy;
        m_view.setEnemyTurnStatus();
    }

    m_dirPath.clear();
    m_network.sendEndTurn();
}

void Game::onEnemyMove(MoveMsg msg)
{
    if (m_currentTurn == Turn::User) {
        throw std::invalid_argument{"Enemy move in user turn."};
    }

    const auto status = m_board.moveBall(msg.dir);
    m_view.drawBoard();

    if (status == MoveStatus::Illegal) {
        throw std::invalid_argument{"Illegal move."};
    }

    m_enemyStatus = status;
    m_dirPath.push_back(msg.dir);
}

void Game::onEnemyUndoMove(UndoMoveMsg)
{
    if (m_currentTurn == Turn::User or m_dirPath.empty()) {
        return;
    }

    const auto dir = m_dirPath.back();
    m_dirPath.pop_back();
    const auto reverseDir = reverseDirection(dir);
    m_board.undoBallMove(reverseDir);
    m_enemyStatus = MoveStatus::Continue;
    m_view.drawBoard();
}

void Game::onEnemyEndTurn(EndTurnMsg)
{
    if (m_currentTurn == Turn::User) {
        throw std::invalid_argument{"Enemy end turn in user turn."};
    }

    if (m_enemyStatus == MoveStatus::Continue) {
        throw std::invalid_argument{"Enemy end turn but doesn't make all moves."};
    }

    if (isGoalToOwn(m_enemyStatus, m_userGoal)) {
        m_match = MatchStatus::GameEnd;
        m_view.setLostStatus();
    } else if (m_enemyStatus == MoveStatus::DeadEnd or isGoalToEnemy(m_enemyStatus, m_userGoal)) {
        m_match = MatchStatus::GameEnd;
        m_view.setWinStatus();
    } else {
        m_currentTurn = Turn::User;
        m_userStatus = MoveStatus::Continue;
        m_view.setContinueStatus();
    }

    m_dirPath.clear();
}

void Game::onEnemyReadyForNewGame(ReadyForNewGameMsg)
{
    if (m_match == MatchStatus::GameEnd) {
        m_match = MatchStatus::EnemyReadyForNew;
    } else if (m_match == MatchStatus::ReadyForNew) {
        initNewGame();
    }
}

void Game::setCurrentTurn(Turn turn)
{
    m_currentTurn = turn;
}

Turn Game::getCurrentTurn() const
{
    return m_currentTurn;
}

void Game::setFirstTurn(Turn turn)
{
    m_firstTurn = turn;
}

void Game::setUserStatus(MoveStatus status)
{
    m_userStatus = status;
}

MoveStatus Game::getUserStatus() const
{
    return m_userStatus;
}

void Game::setEnemyStatus(MoveStatus status)
{
    m_enemyStatus = status;
}

MoveStatus Game::getEnemyStatus() const
{
    return m_enemyStatus;
}

void Game::setUserGoal(Goal goal)
{
    m_userGoal = goal;
}

void Game::setMatchStatus(MatchStatus status)
{
    m_match = status;
}

MatchStatus Game::getMatchStatus() const
{
    return m_match;
}

void Game::setDirectionPath(std::vector<Direction> dirPath)
{
    m_dirPath = dirPath;
}

std::vector<Direction> Game::getDirectionPath() const
{
    return m_dirPath;
}

} // namespace PaperSoccer
