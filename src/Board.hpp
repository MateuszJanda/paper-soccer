#pragma once

#include <cstddef>
#include <vector>
#include "Node.hpp"

namespace PaperSoccer {

enum MoveStatus
{
    Illegal,
    Continue,
    Stop,
    DeadEnd,
    TopGoal,
    BottomGoal
};

class Board
{
public:
    Board(std::size_t width, std::size_t height);
    std::size_t getWidth() const;
    std::size_t getHeight() const;
    void setBallPosition(Position pos);
    Position getBallPosition() const;
    MoveStatus moveBall(Direction dir);

private:
    void setBorders();
    void setHorizontalBorders();
    void setTopBorders();
    void setTopGaol();
    void setBottomBorders();

    bool canReachGoal(Direction dir, int line) const;
    bool isDeadEnd() const;
    bool isPositionInGraph(Position pos) const;

    Position ballPos;
    std::vector<std::vector<Node>> graph;
    const std::size_t goalpostLeft;
    const std::size_t goalpostRight;
};

} // namespace PaperSoccer
