#pragma once

#include "IBoard.hpp"
#include "INCurses.hpp"
#include "View.hpp"

namespace PaperSoccer {

class Game {
public:
    Game(IBoard& board, INCurses& ncurses, View &view);

    void run();

private:
    IBoard& m_board;
    INCurses& m_ncurses;
    View& m_view;
};

}
