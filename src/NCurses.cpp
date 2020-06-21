#include "NCurses.hpp"
#include <clocale>
#include <ncurses.h>

namespace PaperSoccer {

NCurses::NCurses()
{
    // https://stackoverflow.com/questions/59341959/schedule-an-asynchronous-event-that-will-complete-when-stdin-has-waiting-data-in

    std::setlocale(LC_ALL, "");
    initscr();
    noecho();
    // Set the cursor mode as invisible
    curs_set(0);

    set_escdelay(0);
    // If delay is zero, then non-blocking read is used (i.e., read returns ERR if no input is waiting)
    timeout(0);
    // The cbreak routine disables line buffering and erase/kill character processing
    cbreak();

    keypad(stdscr, TRUE);
    mousemask(BUTTON1_PRESSED | BUTTON2_PRESSED, nullptr);

    refresh();
}

NCurses::~NCurses()
{
    endwin();
}

void NCurses::print(int x, int y, std::string str) const
{
    const int pair{0};
    auto ret = attr_set(A_NORMAL, (short)pair, (void*)&pair);
    if (ret) {
        throw std::runtime_error{"ncurses attr_set() error"};
    }

    ret = mvprintw(y, x, str.c_str());
    if (ret) {
        throw std::runtime_error{"ncurses mvprintw() error"};
    }
}

std::optional<Input> NCurses::getInput() const noexcept
{
    const int key = getch();
    MEVENT event;

    if (key == ERR) {
        return std::nullopt;
    } else if (key == '\n') {
        return EnterInput{};
    }
    // Detect left-click
    else if (key == KEY_MOUSE and getmouse(&event) == OK and (event.bstate & BUTTON1_PRESSED)) {
        return MouseInput{.x = event.x, .y = event.y};
    }

    return KeyInput{.key = key};
}

void NCurses::refreshView() const noexcept
{
    refresh();
}

void NCurses::clearView() const noexcept
{
    clear();
}

} // namespace PaperSoccer
