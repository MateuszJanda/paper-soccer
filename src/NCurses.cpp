// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#include "NCurses.hpp"
#include <clocale>
#include <tuple>
#include <ncurses.h>

namespace PaperSoccer {

namespace {
    std::tuple<int, int, int> rgbToCursesColor(int red, int green, int blue)
    {
        return std::make_tuple<int, int, int>(1000 * red/255, 1000 * green/255, 1000 * blue/255);
    }
} // namespace anonymous

NCurses::NCurses()
{
    // https://stackoverflow.com/questions/59341959/schedule-an-asynchronous-event-that-will-complete-when-stdin-has-waiting-data-in

    std::setlocale(LC_ALL, "");
    initscr();
    start_color();
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

    initColors();

    refresh();
}

NCurses::~NCurses()
{
    endwin();
}

void NCurses::initColors() const
{
    prepareColor(1, 0x00, 0x00, 0x00);
    prepareColor(2, 0xff, 0x00, 0x00);

    auto ret = assume_default_colors(2, 1);
    if (ret) {
        throw std::runtime_error{"ncurses assume_default_colors() error"};
    }
}

void NCurses::prepareColor(int colorId, int red, int green, int blue) const
{
    auto [r, g, b] = rgbToCursesColor(red, green, blue);

    auto ret = init_extended_color(colorId, r, g, b);
    if (ret) {
        throw std::runtime_error{"ncurses init_extended_color() - error for id: " + std::to_string(colorId)};
    }
}

void NCurses::print(int x, int y, std::string str, int colorId) const
{
    auto ret = attr_set(A_NORMAL, (short)colorId, (void*)&colorId);
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
