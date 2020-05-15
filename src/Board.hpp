#pragma once

#include <cstddef>
#include <array>

class Board
{
public:
    Board(const std::size_t width, const std::size_t height);

private:
    const std::size_t width;
    const std::size_t height;

};
