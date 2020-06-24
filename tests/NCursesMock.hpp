// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#ifndef NCURSES_MOCK_HPP
#define NCURSES_MOCK_HPP

#include "INCurses.hpp"
#include "gmock/gmock.h"

namespace PaperSoccer {

class NCursesMock : public INCurses {
public:
    MOCK_METHOD(void, print, (int, int, std::string, ColorPair), (const));
    MOCK_METHOD(std::optional<Input>, getInput, (), (const, noexcept));
    MOCK_METHOD(void, refreshView, (), (const, noexcept));
    MOCK_METHOD(void, clearView, (), (const, noexcept));
};

} // namespace PaperSoccer

#endif // NCURSES_MOCK_HPP
