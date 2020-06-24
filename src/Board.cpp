// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#include "Board.hpp"

namespace PaperSoccer {

namespace {
    constexpr std::size_t X_OFFSET{1};
    constexpr std::size_t Y_OFFSET{1 + 2};
    constexpr std::size_t GOAL_WIDTH{2};
} // namespace anonymous

Board::Board(std::size_t width, std::size_t height)
    : m_width{width}
    , m_height{height}
    , m_goalpostLeft{((width + X_OFFSET) / 2) - (GOAL_WIDTH / 2)}
    , m_goalpostRight{((width + X_OFFSET) / 2) + (GOAL_WIDTH / 2)}
    , m_ballPos{static_cast<int>((width + X_OFFSET) / 2), static_cast<int>((height + Y_OFFSET) / 2)}
{
    reset();
}

void Board::reset()
{
    if (m_width < 4 or m_width % 2 == 1 or m_height < 4 or m_height % 2 == 1) {
        throw std::range_error{"Can't build border with this dimensions."};
    }

    // Empty graph
    m_graph.clear();
    for (std::size_t i = 0; i < m_height + Y_OFFSET; i++) {
        m_graph.push_back(std::vector{m_width + X_OFFSET, Node{}});
    }

    setBorders();
    setBallPosition(Position{static_cast<int>((m_width + X_OFFSET) / 2), static_cast<int>((m_height + Y_OFFSET) / 2)});
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
        leftNode.addNeighbours({Direction::Top, Direction::TopLeft, Direction::Left,
            Direction::BottomLeft, Direction::Bottom});

        auto& rightNode = row[row.size() - 1];
        rightNode.addNeighbours({Direction::Top, Direction::TopRight, Direction::Right,
            Direction::BottomRight, Direction::Bottom});
    }
}

void Board::setTopBorders()
{
    // Left borders
    auto& netLine = m_graph[0];
    auto& borderLine = m_graph[1];

    for (std::size_t i = 0; i < m_goalpostLeft; i++) {
        auto& borderNode = borderLine[i];
        borderNode.addNeighbours({Direction::Left, Direction::TopLeft, Direction::Top,
            Direction::TopRight, Direction::Right});

        auto& netNode = netLine[i];
        netNode.addNeighbours({Direction::Left, Direction::TopLeft, Direction::Top,
            Direction::TopRight, Direction::Right, Direction::BottomRight, Direction::Bottom,
            Direction::BottomLeft});
    }

    // Right borders
    for (std::size_t i = m_goalpostRight + 1; i < netLine.size(); i++) {
        auto& borderNode = borderLine[i];
        borderNode.addNeighbours({Direction::Left, Direction::TopLeft, Direction::Top,
            Direction::TopRight, Direction::Right});

        auto& netNode = netLine[i];
        netNode.addNeighbours({Direction::Left, Direction::TopLeft, Direction::Top,
            Direction::TopRight, Direction::Right, Direction::BottomRight, Direction::Bottom,
            Direction::BottomLeft});
    }
}

void Board::setTopGaol()
{
    auto& netLine = m_graph[0];
    auto& borderLine = m_graph[1];

    // Goal corners
    auto& node1 = borderLine[m_goalpostLeft];
    node1.addNeighbours({Direction::Left, Direction::TopLeft, Direction::Top});

    auto& node2 = netLine[m_goalpostLeft];
    node2.addNeighbours({Direction::Bottom, Direction::BottomLeft, Direction::Left,
        Direction::TopLeft, Direction::Top, Direction::TopRight, Direction::Right});

    auto& node3 = borderLine[m_goalpostRight];
    node3.addNeighbours({Direction::Top, Direction::TopRight, Direction::Right});

    auto& node4 = netLine[m_goalpostRight];
    node4.addNeighbours({Direction::Left, Direction::TopLeft, Direction::Top,
        Direction::TopRight, Direction::Right, Direction::BottomRight, Direction::Bottom});

    // Goal net
    for (int i = m_goalpostLeft + 1; i < m_goalpostRight; i++) {
        auto& node = netLine[i];
        node.addNeighbours({Direction::Left, Direction::TopLeft, Direction::Top,
            Direction::TopRight, Direction::Right});
    }
}

void Board::setBottomBorders()
{
    // Left borders
    auto& netLine = m_graph[m_graph.size() - 1];
    auto& borderLine = m_graph[m_graph.size() - 2];

    for (std::size_t i = 0; i < m_goalpostLeft; i++) {
        auto& borderNode = borderLine[i];
        borderNode.addNeighbours({Direction::Right, Direction::BottomRight, Direction::Bottom,
            Direction::BottomLeft, Direction::Left});

        auto& netNode = netLine[i];
        netNode.addNeighbours({Direction::Left, Direction::TopLeft, Direction::Top,
            Direction::TopRight, Direction::Right, Direction::BottomRight, Direction::Bottom,
            Direction::BottomLeft});
    }

    // Right borders
    for (std::size_t i = m_goalpostRight + 1; i < netLine.size(); i++) {
        auto& borderNode = borderLine[i];
        borderNode.addNeighbours({Direction::Right, Direction::BottomRight, Direction::Bottom,
            Direction::BottomLeft, Direction::Left});

        auto& netNode = netLine[i];
        netNode.addNeighbours({Direction::Left, Direction::TopLeft, Direction::Top,
            Direction::TopRight, Direction::Right, Direction::BottomRight, Direction::Bottom,
            Direction::BottomLeft});
    }
}

void Board::setBottomGaol()
{
    auto& netLine = m_graph[m_graph.size() - 1];
    auto& borderLine = m_graph[m_graph.size() - 2];

    // Goal corners
    auto& node1 = borderLine[m_goalpostLeft];
    node1.addNeighbours({Direction::Bottom, Direction::BottomLeft, Direction::Left});

    auto& node2 = netLine[m_goalpostLeft];
    node2.addNeighbours({Direction::Right, Direction::BottomRight, Direction::Bottom,
        Direction::BottomLeft, Direction::Left, Direction::TopLeft, Direction::Top});

    auto& node3 = borderLine[m_goalpostRight];
    node3.addNeighbours({Direction::Right, Direction::BottomRight, Direction::Bottom});

    auto& node4 = netLine[m_goalpostRight];
    node4.addNeighbours({Direction::Top, Direction::TopRight, Direction::BottomRight,
        Direction::Bottom, Direction::BottomLeft, Direction::Right, Direction::Left});

    // Goal net
    for (std::size_t i = m_goalpostLeft + 1; i < m_goalpostRight; i++) {
        auto& node = netLine[i];
        node.addNeighbours({Direction::Right, Direction::BottomRight, Direction::Bottom,
            Direction::BottomLeft, Direction::Left});
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
std::size_t Board::getGoalpostLeft() const
{
    return m_goalpostLeft;
}

std::size_t Board::getGoalpostRight() const
{
    return m_goalpostRight;
}

void Board::setBallPosition(Position pos)
{
    if (not isPositionInGraph(pos)) {
        throw std::out_of_range{"Position out of graph."};
    }
    m_ballPos = pos;
}

Position Board::getBallPosition() const
{
    return m_ballPos;
}

bool Board::hasNeighbour(Position pos, Direction dir) const
{
    if (not isPositionInGraph(pos)) {
        return false;
    }

    const auto& node = m_graph[pos.y][pos.x];
    return node.hasNeighbour(dir);
}

bool Board::hasAllNeighbours(Position pos) const
{
    if (not isPositionInGraph(pos)) {
        return false;
    }
    const auto& node = m_graph[pos.y][pos.x];
    return node.isFull();
}

bool Board::hasAnyNeighbour(Position pos) const
{
    if (not isPositionInGraph(pos)) {
        return false;
    }
    const auto& node = m_graph[pos.y][pos.x];
    return not node.isLonely();
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
        throw std::out_of_range{"New position out of graph."};
    }

    m_graph[m_ballPos.y][m_ballPos.x].addNeighbour(dir);
    const auto reverseDir = reverseDirection(dir);
    m_graph[newPos.y][newPos.x].addNeighbour(reverseDir);
    setBallPosition(newPos);
}

bool Board::undoBallMove(Direction dir)
{
    auto& currentNode = m_graph[m_ballPos.y][m_ballPos.x];
    if (not currentNode.hasNeighbour(dir)) {
        return false;
    }

    const auto prevPos = directionToPosition(m_ballPos, dir);
    if (not isPositionInGraph(prevPos)) {
        return false;
    }

    currentNode.delNeighbour(dir);
    const auto reverseDir = reverseDirection(dir);
    m_graph[prevPos.y][prevPos.x].delNeighbour(reverseDir);
    setBallPosition(prevPos);
    return true;
}

bool Board::canReachGoal(Direction dir, int netLine) const
{
    const auto newPos = directionToPosition(m_ballPos, dir);
    for (std::size_t x = m_goalpostLeft; x <= m_goalpostRight; x++) {
        if (newPos.y == netLine and newPos.x == x) {
            return true;
        }
    }

    return false;
}

bool Board::isDeadEnd() const
{
    for (const auto dir : ALL_DIRS) {
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
    return pos.y >= 0 and pos.y < getHeight() and pos.x >= 0 and pos.x < getWidth();
}

} // namespace PaperSoccer
