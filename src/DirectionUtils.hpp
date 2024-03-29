// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#ifndef DIRECTION_UTILS_HPP
#define DIRECTION_UTILS_HPP

#include "Position.hpp"
#include "proto/Direction.pb.h"
#include <cstdint>

namespace PaperSoccer {

constexpr Position directionToPosition(Position pos, Direction dir)
{
    switch (dir) {
    case Direction::Top:
        pos += std::make_pair(0, -1);
        break;
    case Direction::TopRight:
        pos += std::make_pair(1, -1);
        break;
    case Direction::Right:
        pos += std::make_pair(1, 0);
        break;
    case Direction::BottomRight:
        pos += std::make_pair(1, 1);
        break;
    case Direction::Bottom:
        pos += std::make_pair(0, 1);
        break;
    case Direction::BottomLeft:
        pos += std::make_pair(-1, 1);
        break;
    case Direction::Left:
        pos += std::make_pair(-1, 0);
        break;
    case Direction::TopLeft:
        pos += std::make_pair(-1, -1);
        break;
    default:
        break;
    }

    return pos;
}

constexpr Direction reverseDirection(Direction dir)
{
    switch (dir) {
    case Direction::Top:
        return Direction::Bottom;
    case Direction::TopRight:
        return Direction::BottomLeft;
    case Direction::Right:
        return Direction::Left;
    case Direction::BottomRight:
        return Direction::TopLeft;
    case Direction::Bottom:
        return Direction::Top;
    case Direction::BottomLeft:
        return Direction::TopRight;
    case Direction::Left:
        return Direction::Right;
    case Direction::TopLeft:
        return Direction::BottomRight;
    default:
        break;
    }

    return dir;
}

} // namespace PaperSoccer

#endif // DIRECTION_UTILS_HPP
