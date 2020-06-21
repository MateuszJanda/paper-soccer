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
    MOCK_CONST_METHOD3(print, void(int, int, std::string));
    MOCK_CONST_METHOD0(getInput, std::optional<Input>());
    MOCK_CONST_METHOD0(refreshView, void());
    MOCK_CONST_METHOD0(clearView, void());
};

} // namespace PaperSoccer

#endif // NCURSES_MOCK_HPP
