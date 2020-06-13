#include "Game.hpp"
#include <sstream>
#include <iostream>
#include "NCurses.hpp"

namespace PaperSoccer {

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

Game::Game(INetwork& network, IBoard& board, INCurses& ncurses, IView& view)
    : m_network{network}
    , m_board{board}
    , m_ncurses{ncurses}
    , m_view{view}
    , m_keyMap{{'q', Direction::TopLeft},
          {'u', Direction::TopLeft},
          {'i', Direction::Top},
          {'o', Direction::TopRight},
          {'j', Direction::Left},
          {'l', Direction::Right},
          {'m', Direction::BottomLeft},
          {',', Direction::Bottom},
          {'.', Direction::BottomRight}}
{
}

void Game::run()
{
    using namespace std::placeholders;

    m_network.registerHandlers(std::bind(&Game::onKeyboardMouseInput, this),
        std::bind(&Game::onInitNewGame, this),
        std::bind(&Game::onNewGame, this, _1, _2),
        std::bind(&Game::onEnemyMove, this, _1),
        std::bind(&Game::onEnemyEndTurn, this));
    m_network.run();
}

void Game::onInitNewGame()
{
    m_firstTurn = (m_firstTurn == Turn::Enemy) ? Turn::User : Turn::Enemy;
    m_currentTurn = m_firstTurn;
    m_userGoal = Goal::Top;
    m_userStatus = MoveStatus::Continue;
    m_enemyStatus = MoveStatus::Continue;

    m_board.reset();
    m_view.drawBoard();

    if (m_currentTurn == Turn::User)
    {
        m_view.setContinueStatus();
    }
    else
    {
        m_view.setEnemyTurnStatus();
    }

    Turn turnForEnemy = (m_firstTurn == Turn::Enemy) ? Turn::User : Turn::Enemy;
    Goal enemyGoal = (m_userGoal == Goal::Top) ? Goal::Bottom : Goal::Top;
    m_network.sendNewGame(turnForEnemy, enemyGoal);
}

void Game::onNewGame(const Turn &firstTurn, const Goal &userGoal)
{
    m_firstTurn = firstTurn;
    m_currentTurn = m_firstTurn;
    m_userGoal = userGoal;
    m_userStatus = MoveStatus::Continue;
    m_enemyStatus = MoveStatus::Continue;

    m_board.reset();
    m_view.drawBoard();

    if (m_currentTurn == Turn::User)
    {
        m_view.setContinueStatus();
    }
    else
    {
        m_view.setEnemyTurnStatus();
    }
}

void Game::onKeyboardMouseInput()
{
    while (true) {
        auto input = m_ncurses.getInput();

        if (not input)
            break;

        std::visit(overloaded {
            [this](const KeyInput& data) { userKey(data.key); },
            [this](const EnterInput& data) { userEndTurn(); },
            [this](const MouseInput& data) {  },
        }, *input);
    }
}

void Game::userKey(int key)
{
    if (m_keyMap.contains(key)) {
        Direction dir = m_keyMap.at(key);
        userMove(dir);
    }
}

void Game::userMove(Direction dir)
{
    if (m_currentTurn != Turn::User or m_userStatus != MoveStatus::Continue) {
        return;
    }

    const auto status = m_board.moveBall(dir);
    m_view.drawBoard();

    if (status == MoveStatus::Illegal) {
        return;
    }

    m_userStatus = status;

    if (m_userStatus != MoveStatus::Continue)
    {
        m_view.setReadyToEndTurnStatus();
    }

    m_network.sendMove(dir);
}

void Game::userEndTurn()
{
    if (m_currentTurn != Turn::User or m_userStatus == MoveStatus::Continue) {
        return;
    }

    if (m_userStatus == MoveStatus::DeadEnd or
            (m_userStatus == MoveStatus::TopGoal and m_userGoal == Goal::Top) or
            (m_userStatus == MoveStatus::BottomGoal and m_userGoal == Goal::Bottom))
    {
        m_currentTurn = Turn::None;
        m_view.setLostStatus();
    }
    else if ((m_userStatus == MoveStatus::TopGoal and m_userGoal == Goal::Bottom) or
             (m_userStatus == MoveStatus::BottomGoal and m_userGoal == Goal::Top))
    {
        m_currentTurn = Turn::None;
        m_view.setWinStatus();
    }
    else
    {
        m_currentTurn = Turn::Enemy;
        m_view.setEnemyTurnStatus();
    }

    m_network.sendEndTurn();
}

void Game::onEnemyMove(const Direction& dir)
{
    if (m_currentTurn == Turn::User) {
        throw std::invalid_argument{"Enemy move in user turn."};
    }

    const auto status = m_board.moveBall(dir);
    m_view.drawBoard();

    if (status == MoveStatus::Illegal) {
        throw std::invalid_argument{"Illegal move."};
    }

    m_enemyStatus = status;
}

void Game::onEnemyEndTurn()
{
    if (m_currentTurn == Turn::User) {
        throw std::invalid_argument{"Enemy end turn in user turn."};
    }

    if (m_enemyStatus == MoveStatus::Continue) {
        throw std::invalid_argument{"Enemy end turn but doesn't make all moves."};
    }

    if (
            (m_enemyStatus == MoveStatus::TopGoal and m_userGoal == Goal::Top) or
            (m_enemyStatus == MoveStatus::BottomGoal and m_userGoal == Goal::Bottom))
    {
        m_currentTurn = Turn::None;
        m_view.setLostStatus();
    }
    else if (m_enemyStatus == MoveStatus::DeadEnd or
             (m_enemyStatus == MoveStatus::TopGoal and m_userGoal == Goal::Bottom) or
             (m_enemyStatus == MoveStatus::BottomGoal and m_userGoal == Goal::Top))
    {
        m_currentTurn = Turn::None;
        m_view.setWinStatus();
    }
    else
    {
        m_currentTurn = Turn::User;
        m_userStatus = MoveStatus::Continue;
        m_view.setContinueStatus();
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

void Game::setUserGoal(Goal goal)
{
    m_userGoal = goal;
}

} // namespace PaperSoccer
