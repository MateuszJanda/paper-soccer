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
    void onNewGame(const Turn& firstTurn, const Goal& userGoal);

    void onKeyboardMouseInput();
    void userKey(int key);
    void userMove(Direction dir);
    void userEndTurn();
    void userRequestNewGame();

    void onEnemyMove(const Direction& dir);
    void onEnemyEndTurn();
    void onEnemyReadyForNewGame();

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

    const char NEW_GAME_KEY;
    const std::map<char, Direction> DIR_KEYS;

    MatchStatus m_match{MatchStatus::Connecting};
    Turn m_firstTurn{Turn::User};
    Turn m_currentTurn{Turn::User};
    Goal m_userGoal{Goal::Top};
    MoveStatus m_userStatus{MoveStatus::Continue};
    MoveStatus m_enemyStatus{MoveStatus::Continue};
};

} // namespace PaperSoccer

#endif // GAME_HPP
