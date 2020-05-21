#include "Board.hpp"
#include <cassert>
#include <iostream>

namespace PaperSoccer {

namespace {
    constexpr std::size_t X_OFFSET { 1 };
    constexpr std::size_t Y_OFFSET { 1 + 2 };
    constexpr std::size_t GATE_WIDTH { 2 };
}

Board::Board(std::size_t width, std::size_t height)
    : m_ballPos { static_cast<int>((width + X_OFFSET) / 2), static_cast<int>((height + Y_OFFSET) / 2) }
    , m_goalpostLeft { ((width + X_OFFSET) / 2) - (GATE_WIDTH / 2) }
    , m_goalpostRight { ((width + X_OFFSET) / 2) + (GATE_WIDTH / 2) + 1 }
{
    if (width < 4 or width % 2 == 1 or height < 4 or height % 2 == 1) {
        throw std::range_error("Can't build border with this dimensions.");
    }

    // Empty graph
    for (std::size_t i = 0; i < height + Y_OFFSET; i++) {
        m_graph.push_back(std::vector { width + X_OFFSET, Node {} });
    }

    setBorders();
}

void Board::setBorders()
{
    setHorizontalBorders();
    setTopBorders();
    setTopGaol();
    setBottomBorders();
    setBottomGaol();
}

void Board::setHorizontalBorders()
{
    for (auto& row : m_graph) {
        auto& leftNode = row[0];
        leftNode.addNeighbours({ Direction::Top, Direction::TopLeft, Direction::Left,
            Direction::BottomLeft, Direction::Bottom });

        auto& rightNode = row[row.size() - 1];
        rightNode.addNeighbours({ Direction::Top, Direction::TopRight, Direction::Right,
            Direction::BottomRight, Direction::Bottom });
    }
}

void Board::setTopBorders()
{
    // Left borders
    auto& goalLine = m_graph[0];
    auto& borderLine = m_graph[1];

    for (std::size_t i = 0; i < m_goalpostLeft; i++) {
        auto& borderNode = borderLine[i];
        borderNode.addNeighbours({ Direction::Left, Direction::TopLeft, Direction::Top,
            Direction::TopRight, Direction::Right });

        auto& goalNode = goalLine[i];
        goalNode.addNeighbours({ Direction::Left, Direction::TopLeft, Direction::Top,
            Direction::TopRight, Direction::Right, Direction::BottomRight, Direction::Bottom,
            Direction::BottomLeft });
    }

    // Right borders
    for (std::size_t i = m_goalpostRight; i < goalLine.size(); i++) {
        auto& borderNode = borderLine[i];
        borderNode.addNeighbours({ Direction::Left, Direction::TopLeft, Direction::Top,
            Direction::TopRight, Direction::Right });

        auto& goalNode = goalLine[i];
        goalNode.addNeighbours({ Direction::Left, Direction::TopLeft, Direction::Top,
            Direction::TopRight, Direction::Right, Direction::BottomRight, Direction::Bottom,
            Direction::BottomLeft });
    }
}

void Board::setTopGaol()
{
    auto& goalLine = m_graph[0];
    auto& borderLine = m_graph[1];

    // Goal corners
    auto& node1 = borderLine[m_goalpostLeft];
    node1.addNeighbours({ Direction::Left, Direction::TopLeft, Direction::Top });

    auto& node2 = goalLine[m_goalpostLeft];
    node2.addNeighbours({ Direction::Bottom, Direction::BottomLeft, Direction::Left,
        Direction::TopLeft, Direction::Top, Direction::TopRight, Direction::Right });

    auto& node3 = borderLine[m_goalpostRight];
    node3.addNeighbours({ Direction::Top, Direction::TopLeft, Direction::Right });

    auto& node4 = goalLine[m_goalpostRight];
    node4.addNeighbours({ Direction::Left, Direction::TopLeft, Direction::Top,
        Direction::TopRight, Direction::Right, Direction::BottomRight, Direction::Bottom });

    // Goal net
    for (int i = m_goalpostLeft; i <= m_goalpostRight; i++) {
        auto& node = goalLine[i];
        node.addNeighbours({ Direction::Left, Direction::TopLeft, Direction::Top,
            Direction::TopRight, Direction::Right });
    }
}

void Board::setBottomBorders()
{
    // Left borders
    auto& goalLine = m_graph[m_graph.size() - 1];
    auto& borderLine = m_graph[m_graph.size() - 2];

    for (std::size_t i = 0; i < m_goalpostLeft; i++) {
        auto& borderNode = borderLine[i];
        borderNode.addNeighbours({ Direction::Right, Direction::BottomRight, Direction::Bottom,
            Direction::BottomLeft, Direction::Left });

        auto& goalNode = goalLine[i];
        goalNode.addNeighbours({ Direction::Left, Direction::TopLeft, Direction::Top,
            Direction::TopRight, Direction::Right, Direction::BottomRight, Direction::Bottom,
            Direction::BottomLeft });
    }

    // Right borders
    for (std::size_t i = m_goalpostRight; i < goalLine.size(); i++) {
        auto& borderNode = borderLine[i];
        borderNode.addNeighbours({ Direction::Right, Direction::BottomRight, Direction::Bottom,
            Direction::BottomLeft, Direction::Left });

        auto& goalNode = goalLine[i];
        goalNode.addNeighbours({ Direction::Left, Direction::TopLeft, Direction::Top,
            Direction::TopRight, Direction::Right, Direction::BottomRight, Direction::Bottom,
            Direction::BottomLeft });
    }
}

void Board::setBottomGaol()
{
    auto& goalLine = m_graph[m_graph.size() - 1];
    auto& borderLine = m_graph[m_graph.size() - 2];

    // Goal corners
    auto& node1 = borderLine[m_goalpostLeft];
    node1.addNeighbours({ Direction::Bottom, Direction::BottomLeft, Direction::Left });

    auto& node2 = goalLine[m_goalpostLeft];
    node2.addNeighbours({ Direction::Right, Direction::BottomRight, Direction::Bottom,
        Direction::BottomLeft, Direction::Left, Direction::TopLeft, Direction::Top });

    auto& node3 = borderLine[m_goalpostRight];
    node3.addNeighbours({ Direction::Right, Direction::BottomRight, Direction::Bottom });

    auto& node4 = goalLine[m_goalpostRight];
    node4.addNeighbours({ Direction::Top, Direction::TopRight, Direction::BottomRight,
        Direction::Bottom, Direction::BottomLeft, Direction::Right, Direction::Left });

    // Goal net
    for (std::size_t i = m_goalpostLeft; i <= m_goalpostRight; i++) {
        auto& node = goalLine[i];
        node.addNeighbours({ Direction::Right, Direction::BottomRight, Direction::Bottom,
            Direction::BottomLeft, Direction::Left });
    }
}

std::size_t Board::getWidth() const
{
    return m_graph[0].size();
}

std::size_t Board::getHeight() const
{
    return m_graph.size();
}

void Board::setBallPosition(Position pos)
{
    if (not isPositionInGraph(pos)) {
        throw std::out_of_range { "Position out of graph." };
    }
    m_ballPos = pos;
}

Position Board::getBallPosition() const
{
    return m_ballPos;
}

MoveStatus Board::moveBall(Direction dir)
{
    const auto& currentNode = m_graph[m_ballPos.y][m_ballPos.x];
    if (currentNode.hasNeighbour(dir)) {
        return MoveStatus::Illegal;
    }

    auto newPos = directionToPosition(m_ballPos, dir);
    if (not isPositionInGraph(newPos)) {
        return MoveStatus::Illegal;
    }

    const std::size_t topGoalLine = 0;
    if (canReachGoal(dir, topGoalLine)) {
        updateBallAndGraph(dir);
        return MoveStatus::TopGoal;
    }

    const std::size_t bottomGoalLine = m_graph.size() - 1;
    if (canReachGoal(dir, bottomGoalLine)) {
        updateBallAndGraph(dir);
        return MoveStatus::BottomGoal;
    }

    const auto& newNode = m_graph[newPos.y][newPos.x];
    if (newNode.canEnter()) {
        const bool lonely = newNode.isLonely();
        updateBallAndGraph(dir);

        if (isDeadEnd()) {
            return MoveStatus::DeadEnd;
        } else if (lonely) {
            return MoveStatus::Stop;
        }
        return MoveStatus::Continue;
    }

    return MoveStatus::Illegal;
}

void Board::updateBallAndGraph(Direction dir)
{
    const auto newPos = directionToPosition(m_ballPos, dir);
    if (not isPositionInGraph(newPos)) {
        throw std::out_of_range("New position out of graph.");
    }

    m_graph[m_ballPos.y][m_ballPos.x].addNeighbour(dir);
    const auto reverse = reverseDirection(dir);
    m_graph[newPos.y][newPos.x].addNeighbour(reverse);
    setBallPosition(newPos);
}

bool Board::canReachGoal(Direction dir, int goalLine) const
{
    const auto newPos = directionToPosition(m_ballPos, dir);
    for (std::size_t x = m_goalpostLeft; x <= m_goalpostRight; x++) {
        if (newPos.y == goalLine and newPos.x == x) {
            return true;
        }
    }

    return false;
}

bool Board::isDeadEnd() const
{
    const std::array<Direction, 8> allDirs { Direction::Top,
        Direction::TopLeft,
        Direction::Right,
        Direction::BottomRight,
        Direction::Bottom,
        Direction::BottomLeft,
        Direction::Left,
        Direction::TopRight };

    for (const auto dir : allDirs) {
        const auto newPos = directionToPosition(m_ballPos, dir);
        if (not isPositionInGraph(newPos)) {
            continue;
        }

        const auto& newNode = m_graph[newPos.y][newPos.x];
        const auto& node = m_graph[m_ballPos.y][m_ballPos.x];

        if (not node.hasNeighbour(dir) && newNode.canEnter()) {
            return false;
        }
    }

    return true;
}

bool Board::isPositionInGraph(Position pos) const
{
    return pos.y >= 0 and pos.y < m_graph.size() and pos.x >= 0 and pos.x < m_graph[0].size();
}

} // namespace PaperSoccer
