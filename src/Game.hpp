// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#ifndef GAME_HPP
#define GAME_HPP

#include "proto/Goal.pb.h"
#include "IBoard.hpp"
#include "INCurses.hpp"
#include "INetwork.hpp"
#include "ITimer.hpp"
#include "IView.hpp"
#include "proto/Turn.pb.h"
#include <map>

namespace PaperSoccer {

enum class MatchStatus : std::uint32_t {
    Connecting,
    InProgress,
    GameEnd,
    ReadyForNew,
    EnemyReadyForNew,
};

class Game {
public:
    Game(INetwork& network, ITimer& userTimer, ITimer& enemyTimer, IBoard& board,
        const INCurses& ncurses, const IView& view);

    void run();

    void initNewGame(Goal userGoal = Goal::Up);
    void onNewGame(NewGameMsg msg);
    void resetSettings();

    void onKeyboardMouseInput();
    void userKey(int key);
    void userMove(Direction dir);
    void userUndoMove();
    void userMouse(int x, int y);
    void userRequestNewGame();
    void userEndTurn();

    void onEnemyMove(MoveMsg msg);
    void onEnemyUndoMove();
    void onEnemyEndTurn(EndTurnMsg msg);
    void onEnemyReadyForNewGame();

    void onUserTimerTick(std::chrono::milliseconds timeLeft);
    void onEnemyTimerTick(std::chrono::milliseconds timeLeft);
    void onEnemyTimeout();

    static constexpr char NEW_GAME_KEY{'n'};
    static constexpr char UNDO_MOVE_KEY{'u'};
    const std::map<char, Direction> DIR_KEYS{{'q', Direction::TopLeft},
        {'w', Direction::Top},
        {'e', Direction::TopRight},
        {'a', Direction::Left},
        {'d', Direction::Right},
        {'z', Direction::BottomLeft},
        {'x', Direction::Bottom},
        {'c', Direction::BottomRight}};

protected:
    void drawBoard() const;

    INetwork& m_network;
    ITimer& m_userTimer;
    ITimer& m_enemyTimer;
    IBoard& m_board;
    const INCurses& m_ncurses;
    const IView& m_view;

    MatchStatus m_match{MatchStatus::Connecting};
    int m_userScore{0};
    int m_enemyScore{0};
    Turn m_firstTurn{Turn::User};
    Turn m_currentTurn{Turn::User};
    Goal m_userGoal{Goal::Up};
    MoveStatus m_userStatus{MoveStatus::Continue};
    MoveStatus m_enemyStatus{MoveStatus::Continue};
    std::vector<Direction> m_dirPath;
};

} // namespace PaperSoccer

#endif // GAME_HPP
