#pragma once

#include <cstddef>
#include <vector>
#include "Node.hpp"

class Board
{
public:
    Board(unsigned witdth, unsigned height);
    std::size_t getWidth() const;
    std::size_t getHeight() const;

private:
    void setBorders();
    void setBottom();

    std::vector<std::vector<Node>> graph;
};
