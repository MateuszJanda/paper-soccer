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

    setupColors();

    refresh();
}

NCurses::~NCurses() noexcept
{
    endwin();
}

void NCurses::setupColors() const
{
    prepareColor(DEFAULT_BACKGROUND, 0x31, 0x3a, 0x44);
    prepareColor(DEFAULT_FOREGROUND, 0xfe, 0xfe, 0xfe);

    prepareColor(RED, 0xed, 0x37, 0x92);
    prepareColor(BLUE, 0x00, 0x96, 0xdb);
    prepareColor(GRAY, 0x7d, 0x7d, 0x7d);
    prepareColor(DARK_GRAY, 0x63, 0x63, 0x62);
    prepareColor(YELLOW, 0xf2, 0xe2, 0x2e);
    prepareColor(ORANGE, 0xff, 0xad, 0x1f);
    prepareColor(GREEN, 0x2e, 0xe6, 0x25);

    auto ret = assume_default_colors(DEFAULT_FOREGROUND, DEFAULT_BACKGROUND);
    if (ret) {
        throw std::runtime_error{"assume_default_colors() error"};
    }

    prepareColorPair(ColorPair::USER, BLUE, DEFAULT_BACKGROUND);
    prepareColorPair(ColorPair::ENEMY, RED, DEFAULT_BACKGROUND);
    prepareColorPair(ColorPair::MARK_GRAY, DARK_GRAY, DEFAULT_BACKGROUND);
    prepareColorPair(ColorPair::BUTTON_GRAY, GRAY, DEFAULT_BACKGROUND);
    prepareColorPair(ColorPair::BUTTON_YELLOW, YELLOW, DEFAULT_BACKGROUND);
    prepareColorPair(ColorPair::BUTTON_ORANGE, ORANGE, DEFAULT_BACKGROUND);
    prepareColorPair(ColorPair::BUTTON_GREEN, GREEN, DEFAULT_BACKGROUND);
}

void NCurses::prepareColor(int colorId, int red, int green, int blue) const
{
    auto [r, g, b] = rgbToCursesColor(red, green, blue);

    auto ret = init_extended_color(colorId, r, g, b);
    if (ret) {
        throw std::runtime_error{"init_extended_color() error for: " + std::to_string(colorId)};
    }
}

void NCurses::prepareColorPair(ColorPair colorPair, int fg, int bg) const
{
    const auto ret = init_extended_pair(static_cast<int>(colorPair), fg, bg);
    if (ret) {
        throw std::runtime_error{"init_extended_pair() error for: " + std::to_string(static_cast<int>(colorPair))};
    }
}

void NCurses::print(int x, int y, std::string str, ColorPair coloPair) const
{
    const auto pariNum{static_cast<int>(coloPair)};
    auto ret = attr_set(A_NORMAL, (short)pariNum, (void*)&pariNum);
    if (ret) {
        throw std::runtime_error{"attr_set() error"};
    }

    ret = mvprintw(y, x, str.c_str());
    if (ret) {
        throw std::runtime_error{"mvprintw() error"};
    }
}

std::optional<Input> NCurses::getInput() const noexcept
{
    const int key = getch();
    MEVENT event{};

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
