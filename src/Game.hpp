#ifndef GAME_HPP
#define GAME_HPP

#include "IBoard.hpp"
#include "INCurses.hpp"
#include "INetwork.hpp"
#include "TmpMoveMsg.hpp"
#include "IView.hpp"
#include <map>

namespace PaperSoccer {

class Game {
public:
    Game(INetwork& network, IBoard& board, INCurses& ncurses, IView &view);

    void run();
    void onKeyboardMouseInput();
    void onMove(const TmpMoveMsg& msg);
    void makeMove(int key);
    Direction keyToDirection(int c);

private:
    INetwork& m_network;
    IBoard& m_board;
    INCurses& m_ncurses;
    IView& m_view;

    const std::map<char, Direction> m_keyMap;
};

} // namespace PaperSoccer

#endif // GAME_HPP
