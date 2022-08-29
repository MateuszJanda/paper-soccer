// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#ifndef VIEW_UTILS_HPP
#define VIEW_UTILS_HPP

#include <cstdint>

namespace PaperSoccer::ViewUtils {

static constexpr int X_FACTOR{3};
static constexpr int Y_FACTOR{2};

static constexpr unsigned int X_OFFSET{2};
static constexpr unsigned int Y_OFFSET{1};

static constexpr unsigned int Y_TIME_OFFSET{ViewUtils::Y_OFFSET + 5};
static constexpr unsigned int Y_SCORE_OFFSET{ViewUtils::Y_OFFSET + 9};
static constexpr unsigned int Y_LEGEND_OFFSET{ViewUtils::Y_OFFSET + 13};

inline int vx(int x)
{
    return x * X_FACTOR + X_OFFSET;
}

inline int vy(int y)
{
    return y * Y_FACTOR + Y_OFFSET;
}

} // namespace PaperSoccer::ViewUtils

#endif // VIEW_UTILS_HPP
