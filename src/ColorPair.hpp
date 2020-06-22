// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#ifndef COLOR_PAIR_HPP
#define COLOR_PAIR_HPP

namespace PaperSoccer {

enum class ColorPair : int {
    DEFAULT = 0,
    USER,
    ENEMY,
    MARK_GRAY,
    BUTTON_GRAY,
    BUTTON_YELLOW,
    BUTTON_ORANGE,
    BUTTON_GREEN
};

} // namespace PaperSoccer

#endif // COLOR_PAIR_HPP
