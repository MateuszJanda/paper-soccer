#include "NCurses.hpp"
#include <ncurses.h>
#include <locale.h>

namespace PaperSoccer {

NCurses::NCurses()
{
    setlocale(LC_ALL, "");
    initscr();
    printw("Hello World !!!");
    refresh();
}

void NCurses::print(int x, int y, std::string str)
{
    int pair = 0;
    int ret = attr_set(A_NORMAL, (short)pair, (void*)&pair);
    mvprintw(y, x, str.c_str());
}

int NCurses::getChar()
{
    return getch();
}

NCurses::~NCurses()
{
    endwin();
}

}
