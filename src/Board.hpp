// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#ifndef BOARD_HPP
#define BOARD_HPP

#include "IBoard.hpp"
#include "Node.hpp"
#include <array>
#include <vector>

namespace PaperSoccer {

class Board : public IBoard {
public:
    Board(std::size_t width, std::size_t height);
    void reset() override;
    std::size_t getWidth() const override;
    std::size_t getHeight() const override;
    std::size_t getGoalpostLeft() const override;
    std::size_t getGoalpostRight() const override;
    void setBallPosition(Position pos) override;
    Position getBallPosition() const override;
    bool hasNeighbour(Position pos, Direction dir) const override;
    bool hasAllNeighbours(Position pos) const override;
    bool hasAnyNeighbour(Position pos) const override;
    MoveStatus moveBall(Direction dir) override;
    bool undoBallMove(Direction dir) override;

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

    static constexpr std::array<Direction, 8> ALL_DIRS{Direction::Top,
        Direction::TopLeft,
        Direction::Right,
        Direction::BottomRight,
        Direction::Bottom,
        Direction::BottomLeft,
        Direction::Left,
        Direction::TopRight};

    const std::size_t m_width;
    const std::size_t m_height;
    const std::size_t m_goalpostLeft;
    const std::size_t m_goalpostRight;
    Position m_ballPos;
    std::vector<std::vector<Node>> m_graph;
};

} // namespace PaperSoccer

#endif // BOARD_HPP
