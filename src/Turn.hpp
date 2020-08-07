// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#ifndef TURN_HPP
#define TURN_HPP

#include <cstdint>

namespace PaperSoccer {

enum class Turn : std::uint32_t {
    User,
    Enemy,
};

} // namespace PaperSoccer

#endif // TURN_HPP
