#include "NCurses.hpp"
#include <ncurses.h>
#include <clocale>

namespace PaperSoccer {

NCurses::NCurses()
{
    // https://stackoverflow.com/questions/59341959/schedule-an-asynchronous-event-that-will-complete-when-stdin-has-waiting-data-in

    std::setlocale(LC_ALL, "");
    initscr();
    noecho();
    // set the cursor mode - Invisible
    curs_set(0);

//    ESCDELAY = 0;
    set_escdelay(0);
    // If delay is zero, then non-blocking read is used (i.e., read returns ERR if no input is waiting)
    timeout(0);
    // The cbreak routine disables line buffering and erase/kill character-processing
    cbreak();

    keypad(stdscr, TRUE);
    mousemask(BUTTON1_PRESSED | BUTTON2_PRESSED, NULL);

    printw("Hello World !!!");
    refresh();
}

void NCurses::print(int x, int y, std::string str)
{
    int pair = 0;
    int ret = attr_set(A_NORMAL, (short)pair, (void*)&pair);
    mvprintw(y, x, str.c_str());
}

std::tuple<int, int, int> NCurses::getChar()
{
    int c = getch();
    MEVENT event;
    switch(c)
    {
    case KEY_MOUSE:
        if(getmouse(&event) == OK)
        {
//            printw("Click");
            if(event.bstate & BUTTON1_PRESSED) // This works for left-click
            {
                return std::make_tuple(1000, event.x, event.y);
            }
        }
        break;
    default:
        return std::make_tuple(c, 0, 0);
    }

    return std::make_tuple(c, 0, 0);
}

void NCurses::refreshView()
{
    refresh();
}

NCurses::~NCurses()
{
    endwin();
}

}
