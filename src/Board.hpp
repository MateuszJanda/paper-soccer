#pragma once

#include "Node.hpp"
#include <cstddef>
#include <vector>

namespace PaperSoccer {

enum MoveStatus {
    Illegal,
    Continue,
    Stop,
    DeadEnd,
    TopGoal,
    BottomGoal
};

class Board {
public:
    Board(std::size_t width, std::size_t height);
    std::size_t getWidth() const;
    std::size_t getHeight() const;
    std::size_t getGoalpostLeft() const;
    std::size_t getGoalpostRight() const;
    void setBallPosition(Position pos);
    Position getBallPosition() const;
    bool hasNeighbour(Position pos, Direction dir) const;
    MoveStatus moveBall(Direction dir);

private:
    void setBorders();
    void setHorizontalBorders();
    void setTopBorders();
    void setTopGaol();
    void setBottomBorders();
    void setBottomGaol();

    void updateBallAndGraph(Direction dir);
    bool canReachGoal(Direction dir, int goalLine) const;
    bool isDeadEnd() const;
    bool isPositionInGraph(Position pos) const;

    Position m_ballPos;
    std::vector<std::vector<Node>> m_graph;
    const std::size_t m_goalpostLeft;
    const std::size_t m_goalpostRight;
};

} // namespace PaperSoccer
