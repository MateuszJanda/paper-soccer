#ifndef GAME_HPP
#define GAME_HPP

#include "IBoard.hpp"
#include "INCurses.hpp"
#include "INetwork.hpp"
#include "TmpMoveMsg.hpp"
#include "View.hpp"
#include <boost/asio.hpp>

namespace PaperSoccer {

class Game {
public:
    Game(INetwork& network, IBoard& board, INCurses& ncurses, View& view);

    void run();
    void handleKeyboardMouseInput();
    void makeMove(int d, int x, int y);
    void handleMoveMsg(const TmpMoveMsg& msg);
    Direction keyToDirection(int c);

private:
    INetwork& m_network;
    IBoard& m_board;
    INCurses& m_ncurses;
    View& m_view;
};

} // namespace PaperSoccer

#endif // GAME_HPP
