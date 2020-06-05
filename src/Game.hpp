#ifndef GAME_HPP
#define GAME_HPP

#include "IBoard.hpp"
#include "INCurses.hpp"
#include "View.hpp"
#include "INetwork.hpp"
#include "TmpMoveMsg.hpp"
#include <boost/asio.hpp>

namespace PaperSoccer {

class Game {
public:
    Game(INetwork& network, IBoard& board, INCurses& ncurses, View &view);

    void run();
    void handleKeyboardMouseInput();
    void makeMove(int d, int x, int y);
    void handleReadMsg(const TmpMoveMsg &msg);
    Direction keyToDirection(int c);

private:
    INetwork& m_network;
    IBoard& m_board;
    INCurses& m_ncurses;
    View& m_view;
};

}

#endif // GAME_HPP
