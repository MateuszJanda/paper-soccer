#pragma once

#include <cstddef>
#include <array>
#include "Node.hpp"

namespace
{
constexpr std::size_t X_SHIFT{2};
constexpr std::size_t Y_SHIFT{4};
}

template<std::size_t WIDTH, std::size_t HEIGHT>
class Board
{
public:
    Board();

private:
    const std::size_t width{WIDTH};
    const std::size_t height{HEIGHT};

    std::array<std::array<Node, HEIGHT + Y_SHIFT>, WIDTH + X_SHIFT> graph;
};
