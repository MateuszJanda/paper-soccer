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
    Game(INetwork& network, IBoard& board, INCurses& ncurses, IView& view);

    void run();

    void initNewGame();
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
    void onEnemyUndoMove(UndoMoveMsg);
    void onEnemyEndTurn(EndTurnMsg);
    void onEnemyReadyForNewGame(ReadyForNewGameMsg);

    static constexpr char NEW_GAME_KEY{'n'};
    static constexpr char UNDO_MOVE_KEY{'z'};
    const std::map<char, Direction> DIR_KEYS{{'q', Direction::TopLeft},
                                             {'u', Direction::TopLeft},
                                             {'i', Direction::Top},
                                             {'o', Direction::TopRight},
                                             {'j', Direction::Left},
                                             {'l', Direction::Right},
                                             {'m', Direction::BottomLeft},
                                             {',', Direction::Bottom},
                                             {'.', Direction::BottomRight}};

protected:
    INetwork& m_network;
    IBoard& m_board;
    INCurses& m_ncurses;
    IView& m_view;

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
