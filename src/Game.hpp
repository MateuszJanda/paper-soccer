#ifndef GAME_HPP
#define GAME_HPP

#include "IBoard.hpp"
#include "INCurses.hpp"
#include "INetwork.hpp"
#include "TmpMoveMsg.hpp"
#include "IView.hpp"

namespace PaperSoccer {

class Game {
public:
    Game(INetwork& network, IBoard& board, INCurses& ncurses, IView &view);

    void run();
    void handleKeyboardMouseInput();
    void makeMove(int d, int x, int y);
    void handleMoveMsg(const TmpMoveMsg& msg);
    Direction keyToDirection(int c);

private:
    INetwork& m_network;
    IBoard& m_board;
    INCurses& m_ncurses;
    IView& m_view;
};

} // namespace PaperSoccer

#endif // GAME_HPP
