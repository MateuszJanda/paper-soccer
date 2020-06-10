#include "NCurses.hpp"
#include <clocale>
#include <ncurses.h>

namespace PaperSoccer {

void rawPrint(int x, int y, std::string str)
{
    int pair = 0;
    int ret = attr_set(A_NORMAL, (short)pair, (void*)&pair);
    mvprintw(y, x, str.c_str());
}

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

NCurses::~NCurses()
{
    endwin();
}

void NCurses::print(int x, int y, std::string str)
{
    int pair = 0;
    int ret = attr_set(A_NORMAL, (short)pair, (void*)&pair);
    mvprintw(y, x, str.c_str());
}

std::optional<Input> NCurses::getInput()
{
    int key = getch();
    MEVENT event;

    if (key == ERR)
    {
        return std::nullopt;
    }
    // This works for left-click
    else if (key == KEY_MOUSE and getmouse(&event) == OK and (event.bstate & BUTTON1_PRESSED))
    {
        return MouseData{.x=event.x, .y=event.y};
    }

    return KeyData{.key=key};
}

void NCurses::refreshView()
{
    refresh();
}

} // namespace PaperSoccer
