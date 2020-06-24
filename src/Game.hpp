// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#ifndef GAME_HPP
#define GAME_HPP

#include "Goal.hpp"
#include "IBoard.hpp"
#include "INCurses.hpp"
#include "INetwork.hpp"
#include "IView.hpp"
#include "Turn.hpp"
#include <map>

namespace PaperSoccer {

enum class MatchStatus : std::uint8_t {
    Connecting,
    InProgress,
    GameEnd,
    ReadyForNew,
    EnemyReadyForNew,
};


class Game {
public:
    Game(INetwork& network, IBoard& board, const INCurses& ncurses, const IView& view);

    void run();

    void initNewGame(Goal userGoal = Goal::Top);
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
    void onEnemyEndTurn();
    void onEnemyReadyForNewGame();

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
    IBoard& m_board;
    const INCurses& m_ncurses;
    const IView& m_view;

    MatchStatus m_match{MatchStatus::Connecting};
    int m_userScore{0};
    int m_enemyScore{0};
    Turn m_firstTurn{Turn::User};
    Turn m_currentTurn{Turn::User};
    Goal m_userGoal{Goal::Top};
    MoveStatus m_userStatus{MoveStatus::Continue};
    MoveStatus m_enemyStatus{MoveStatus::Continue};
    std::vector<Direction> m_dirPath;
};

} // namespace PaperSoccer

#endif // GAME_HPP
