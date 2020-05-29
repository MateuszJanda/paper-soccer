#pragma once

#include "IBoard.hpp"
#include "INCurses.hpp"
#include "View.hpp"

namespace PaperSoccer {

class Game {
public:
    Game(IBoard& board, INCurses& ncurses, View &view);

    void run();
    void run1();
    void on_input();
    void ddd(int d, int x, int y);

private:
    IBoard& m_board;
    INCurses& m_ncurses;
    View& m_view;
};

}
