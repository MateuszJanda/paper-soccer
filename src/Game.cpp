#include "Game.hpp"
#include <sstream>

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
        std::bind(&Game::onEnemyMove, this, _1));
    m_network.run();
}

void Game::onInitNewGame()
{
    m_firstTurn = (m_firstTurn == Turn::Enemy) ? Turn::User : Turn::Enemy;
    m_currentTurn = m_firstTurn;
    m_userGoal = UserGoal::Top;
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
    UserGoal enemyGoal = (m_userGoal == UserGoal::Top) ? UserGoal::Bottom : UserGoal::Top;
    m_network.sendNewGame(turnForEnemy, enemyGoal);
}

void Game::onNewGame(Turn firstTurn, UserGoal userGoal)
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
    if (m_currentTurn != Turn::User or not m_keyMap.contains(key)) {
        return;
    }

    Direction dir = m_keyMap.at(key);
    const auto status = m_board.moveBall(dir);
    m_view.drawBoard();

    if (status != MoveStatus::Illegal) {
        m_userStatus = status;

        if (m_userStatus != MoveStatus::Continue)
        {
            m_view.setReadyToEndTurnStatus();
        }

        m_network.sendMove(dir);
    }
}

void Game::userEndTurn()
{
    if (m_currentTurn != Turn::User or m_userStatus == MoveStatus::Continue) {
        return;
    }

    if (m_userStatus == MoveStatus::DeadEnd or
            (m_userStatus == MoveStatus::TopGoal and m_userGoal == UserGoal::Top) or
            (m_userStatus == MoveStatus::BottomGoal and m_userGoal == UserGoal::Bottom))
    {
        m_currentTurn = Turn::None;
        m_view.setLostStatus();
    }
    else if ((m_userStatus == MoveStatus::TopGoal and m_userGoal == UserGoal::Bottom) or
             (m_userStatus == MoveStatus::BottomGoal and m_userGoal == UserGoal::Top))
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

    if (m_enemyStatus == MoveStatus::DeadEnd or
            (m_enemyStatus == MoveStatus::TopGoal and m_userGoal == UserGoal::Top) or
            (m_enemyStatus == MoveStatus::BottomGoal and m_userGoal == UserGoal::Bottom))
    {
        m_currentTurn = Turn::None;
        m_view.setWinStatus();
    }
    else if ((m_enemyStatus == MoveStatus::TopGoal and m_userGoal == UserGoal::Bottom) or
             (m_enemyStatus == MoveStatus::BottomGoal and m_userGoal == UserGoal::Top))
    {
        m_currentTurn = Turn::None;
        m_view.setLostStatus();
    }
    else
    {
        m_currentTurn = Turn::User;
        m_view.setContinueStatus();
    }
}

} // namespace PaperSoccer
