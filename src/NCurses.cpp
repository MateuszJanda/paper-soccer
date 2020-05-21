#include "NCurses.hpp"
#include <ncurses.h>

namespace PaperSoccer {

NCurses::NCurses()
{
    initscr();
    printw("Hello World !!!");
    refresh();
}

NCurses::~NCurses()
{
    endwin();
}

}
