// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#ifndef GOAL_HPP
#define GOAL_HPP

#include <cstdint>

namespace PaperSoccer {

enum class Goal : std::uint8_t {
    Top,
    Bottom
};

} // namespace PaperSoccer

#endif // GOAL_HPP
