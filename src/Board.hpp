#pragma once

#include "IBoard.hpp"
#include "Node.hpp"
#include <cstddef>
#include <vector>

namespace PaperSoccer {

class Board : public IBoard {
public:
    Board(std::size_t width, std::size_t height);
    std::size_t getWidth() const override;
    std::size_t getHeight() const override;
    std::size_t getGoalpostLeft() const override;
    std::size_t getGoalpostRight() const override;
    void setBallPosition(Position pos) override;
    Position getBallPosition() const;
    bool hasNeighbour(Position pos, Direction dir) const override;
    MoveStatus moveBall(Direction dir) override;

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
