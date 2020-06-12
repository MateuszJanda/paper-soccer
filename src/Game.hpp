#ifndef GAME_HPP
#define GAME_HPP

#include "IBoard.hpp"
#include "INCurses.hpp"
#include "INetwork.hpp"
#include "IView.hpp"
#include <map>

namespace PaperSoccer {

enum Turn {
    None,
    User,
    Enemy
};

enum UserGoal {
    Top,
    Bottom
};

class Game {
public:
    Game(INetwork& network, IBoard& board, INCurses& ncurses, IView &view);

    void run();
    void onInitNewGame();
    void onNewGame(Turn firstTurn, UserGoal userGoal);

    void onKeyboardMouseInput();

    void userKey(int key);
    void userEndTurn();


    void onEnemyMove(const Direction& dir);
    void onEnemyEndTurn();

private:
    INetwork& m_network;
    IBoard& m_board;
    INCurses& m_ncurses;
    IView& m_view;
    const std::map<char, Direction> m_keyMap;

    Turn m_firstTurn{Turn::None};
    Turn m_currentTurn{Turn::None};
    UserGoal m_userGoal{UserGoal::Top};
    MoveStatus m_userStatus{MoveStatus::Continue};
    MoveStatus m_enemyStatus{MoveStatus::Continue};
};

} // namespace PaperSoccer

#endif // GAME_HPP
