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

    void onKeyboardMouseInput();
    void userKey(int key);
    void userMouse(int x, int y);
    void userMove(Direction dir);
    void userEndTurn();
    void userRequestNewGame();
    void userUndoMove();

    void onEnemyMove(MoveMsg msg);
    void onEnemyUndoMove(MoveMsg msg);
    void onEnemyEndTurn(EndTurnMsg);
    void onEnemyReadyForNewGame(ReadyForNewGameMsg);

    void setCurrentTurn(Turn turn);
    Turn getCurrentTurn() const;
    void setFirstTurn(Turn turn);
    void setUserStatus(MoveStatus status);
    MoveStatus getUserStatus() const;
    void setEnemyStatus(MoveStatus status);
    void setUserGoal(Goal goal);
    void setMatchStatus(MatchStatus status);
    MatchStatus getMatchStatus() const;

private:
    INetwork& m_network;
    IBoard& m_board;
    INCurses& m_ncurses;
    IView& m_view;

    const char NEW_GAME_KEY{'n'};
    const char UNDO_MOVE_KEY{'z'};
    const std::map<char, Direction> DIR_KEYS;

    MatchStatus m_match{MatchStatus::Connecting};
    Turn m_firstTurn{Turn::User};
    Turn m_currentTurn{Turn::User};
    Goal m_userGoal{Goal::Top};
    MoveStatus m_userStatus{MoveStatus::Continue};
    MoveStatus m_enemyStatus{MoveStatus::Continue};
    std::vector<Direction> m_dirPath;
};

} // namespace PaperSoccer

#endif // GAME_HPP
