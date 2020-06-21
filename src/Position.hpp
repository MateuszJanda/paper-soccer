// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#ifndef POSITION_HPP
#define POSITION_HPP

#include <utility>

namespace PaperSoccer {

class Position {
public:
    constexpr Position(int x, int y)
        : x(x)
        , y(y)
    {
    }

    constexpr Position& operator+=(const std::pair<int, int>& offset)
    {
        x += offset.first;
        y += offset.second;
        return *this;
    }

    constexpr bool operator==(const Position& second) const
    {
        return x == second.x and y == second.y;
    }

    int x;
    int y;
};

} // namespace PaperSoccer

#endif // POSITION_HPP
