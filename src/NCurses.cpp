#include "NCurses.hpp"
#include <ncurses.h>

namespace PaperSoccer {

NCurses::NCurses()
{
    initscr();
    printw("Hello World !!!");
    refresh();
}

void NCurses::print(int y, int x, std::string str)
{
    int pair = 0;
    int ret = attr_set(A_NORMAL, (short) pair, (void *) &pair);
    mvprintw(y, x, str.c_str());
    getch();
}

NCurses::~NCurses()
{
    endwin();
}

}
