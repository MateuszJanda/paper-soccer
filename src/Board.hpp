#pragma once

#include <cstddef>
#include <array>
#include "Node.hpp"

namespace
{
constexpr std::size_t X_OFFSET{2*1};
constexpr std::size_t Y_OFFSET{2*2};
}

template<std::size_t WIDTH, std::size_t HEIGHT>
class Board
{
public:
    Board();
    std::size_t getWidth() const;
    std::size_t getHeight() const;

private:
    std::array<std::array<Node, WIDTH + X_OFFSET>, HEIGHT + Y_OFFSET> graph;
};
