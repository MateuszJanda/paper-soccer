// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#include "ViewBoard.hpp"
#include <iomanip>
#include <ranges>
#include <sstream>

namespace PaperSoccer {

ViewBoard::ViewBoard(const IBoard& board, const INCurses& ncurses)
    : m_board{board}
    , m_ncurses{ncurses}
{
}

void ViewBoard::drawBoard(const std::string& topName, ColorPair topColor, const std::string& bottomName, ColorPair bottomColor,
    const std::vector<Direction>& dirPath, ColorPair ballColor) const
{
    for (std::size_t y = 0; y < m_board.getHeight(); y++) {
        for (std::size_t x = 0; x < m_board.getWidth(); x++) {
            Position nodePos{x, y};
            auto [nodeSkip, neighSkip] = filterDirsForOutOfBorder(nodePos);
            auto visability = markerVisability(nodePos);
            drawCell(nodePos, nodeSkip, neighSkip, visability);
        }
    }

    drawNames(topName, topColor, bottomName, bottomColor);
    drawPathMarkers(dirPath, ballColor);
    m_ncurses.refreshView();
}

Skips ViewBoard::filterDirsForOutOfBorder(Position nodePos) const
{
    Skip allNodeSkip;
    Skip allNeighSkip;

    Skip nodeSkip;
    Skip neighSkip;

    std::tie(nodeSkip, neighSkip) = filterDirsForTopNetLine(nodePos);
    allNodeSkip.merge(nodeSkip);
    allNeighSkip.merge(neighSkip);

    std::tie(nodeSkip, neighSkip) = filterDirsForTopBorderLine(nodePos);
    allNodeSkip.merge(nodeSkip);
    allNeighSkip.merge(neighSkip);

    std::tie(nodeSkip, neighSkip) = filterDirsForBottomNetLine(nodePos);
    allNodeSkip.merge(nodeSkip);
    allNeighSkip.merge(neighSkip);

    std::tie(nodeSkip, neighSkip) = filterDirsForRightLine(nodePos);
    allNodeSkip.merge(nodeSkip);
    allNeighSkip.merge(neighSkip);

    return Skips{allNodeSkip, allNeighSkip};
}

Skips ViewBoard::filterDirsForTopNetLine(Position nodePos) const
{
    constexpr auto topNetLine{0};

    if (nodePos.isPositive() and nodePos.y == topNetLine) {
        const std::size_t x = nodePos.x;
        if (x >= m_board.getGoalpostLeft() and x < m_board.getGoalpostRight()) {
            return Skips{Skip{Direction::Top, Direction::TopRight}, Skip{Direction::TopLeft}};
        } else {
            return Skips{Skip{Direction::Top, Direction::TopRight, Direction::Right},
                Skip{Direction::TopLeft}};
        }
    }

    return Skips{};
}

Skips ViewBoard::filterDirsForTopBorderLine(Position nodePos) const
{
    constexpr auto topBorderLine{1};

    if (nodePos.isPositive() and nodePos.y == topBorderLine) {
        const std::size_t x = nodePos.x;
        if (x < m_board.getGoalpostLeft() or x > m_board.getGoalpostRight()) {
            return Skips{Skip{Direction::Top, Direction::TopRight}, Skip{Direction::TopLeft}};
        } else if (x == m_board.getGoalpostRight()) {
            return Skips{Skip{Direction::TopRight}, Skip{Direction::TopLeft}};
        }
    }

    return Skips{};
}

Skips ViewBoard::filterDirsForBottomNetLine(Position nodePos) const
{
    const int bottomNetLine = m_board.getHeight() - 1;

    if (nodePos.isPositive() and nodePos.y == bottomNetLine) {
        const std::size_t x = nodePos.x;
        if (x < m_board.getGoalpostLeft() or x > m_board.getGoalpostRight()) {
            return Skips{Skip{Direction::Top, Direction::TopRight, Direction::Right},
                Skip{Direction::TopLeft}};
        } else if (x == m_board.getGoalpostRight()) {
            return Skips{Skip{Direction::TopRight, Direction::Right}, Skip{Direction::TopLeft}};
        }
    }

    return Skips{};
}

Skips ViewBoard::filterDirsForRightLine(Position nodePos) const
{
    const int rightLine = m_board.getWidth() - 1;

    if (nodePos.x == rightLine) {
        return Skips{Skip{Direction::TopRight, Direction::Right}, Skip{}};
    }

    return Skips{};
}

MarkerVisability ViewBoard::markerVisability(Position nodePos) const
{
    const int bottomLine = m_board.getHeight() - 1;

    if ((nodePos.y == 0 or nodePos.y == bottomLine) and m_board.hasAllNeighbours(nodePos)) {
        return MarkerVisability::Invisible;
    } else if (m_board.hasAnyNeighbour(nodePos)) {
        return MarkerVisability::Occupied;
    }

    return MarkerVisability::NotOccupied;
}

void ViewBoard::drawCell(Position nodePos, Skip nodeSkip, Skip neighSkip, MarkerVisability visability) const
{
    clearLines(nodePos);

    if (not nodeSkip.contains(Direction::Top) and m_board.hasNeighbour(nodePos, Direction::Top)) {
        drawVerticalToTopLine(nodePos);
    }

    if (not nodeSkip.contains(Direction::Right) and m_board.hasNeighbour(nodePos, Direction::Right)) {
        drawHorizontalToRightLine(nodePos);
    }

    auto topRight{false};
    if (not nodeSkip.contains(Direction::TopRight) and m_board.hasNeighbour(nodePos, Direction::TopRight)) {
        topRight = true;
    }

    auto topLeft{false};
    const Position neighbourPos{nodePos.x + 1, nodePos.y};
    if (not neighSkip.contains(Direction::TopLeft) and neighbourPos.x < static_cast<int>(m_board.getWidth())
        and m_board.hasNeighbour(neighbourPos, Direction::TopLeft)) {
        topLeft = true;
    }

    if (topRight and topLeft) {
        drawCrossToRight(nodePos);
    } else if (topRight) {
        drawHypotenuseToTopRight(nodePos);
    } else if (topLeft) {
        drawHypotenuseToTopLeft(neighbourPos);
    }

    drawMarker(nodePos, visability);
}

void ViewBoard::drawVerticalToTopLine(const Position& nodePos) const
{
    m_ncurses.print(ViewUtils::vx(nodePos.x), ViewUtils::vy(nodePos.y) - 1, "|");
}

void ViewBoard::drawHorizontalToRightLine(const Position& nodePos) const
{
    m_ncurses.print(ViewUtils::vx(nodePos.x) + 1, ViewUtils::vy(nodePos.y), "--");
}

void ViewBoard::drawCrossToRight(const Position& nodePos) const
{
    // >< - U+003e U+003f https://en.wikipedia.org/wiki/Basic_Latin_(Unicode_block)
    // ᐳᐸ - U+1433 U+1438 https://en.wikipedia.org/wiki/Unified_Canadian_Aboriginal_Syllabics_(Unicode_block)
    const std::string symbol = "ᐳᐸ";
    m_ncurses.print(ViewUtils::vx(nodePos.x) + 1, ViewUtils::vy(nodePos.y) - 1, symbol);
}

void ViewBoard::drawHypotenuseToTopRight(const Position& nodePos) const
{
    //  / - U+002F        https://en.wikipedia.org/wiki/Basic_Latin_(Unicode_block)
    // ⸝⸍ - U+2e1d U+2e0d https://en.wikipedia.org/wiki/Supplemental_Punctuation
    // ᐟ  - U+141f        https://en.wikipedia.org/wiki/Unified_Canadian_Aboriginal_Syllabics_(Unicode_block)
    // ৴  - U+09f4        https://en.wikipedia.org/wiki/Bengali_(Unicode_block)
    const std::string symbol = "⸝⸍";
    m_ncurses.print(ViewUtils::vx(nodePos.x) + 1, ViewUtils::vy(nodePos.y) - 1, symbol);
}

void ViewBoard::drawHypotenuseToTopLeft(const Position& nodePos) const
{
    //  \ - U+005C        https://en.wikipedia.org/wiki/Basic_Latin_(Unicode_block)
    // ⸌⸜ - U+2e0c U+2e1c https://en.wikipedia.org/wiki/Supplemental_Punctuation
    // ᐠ  - U+1420        https://en.wikipedia.org/wiki/Unified_Canadian_Aboriginal_Syllabics_(Unicode_block)
    // ৲  - U+09f2        https://en.wikipedia.org/wiki/Bengali_(Unicode_block)
    // ヽ  - U+30FD       https://en.wikipedia.org/wiki/Katakana_(Unicode_block)
    const std::string symbol = "⸌⸜";
    m_ncurses.print(ViewUtils::vx(nodePos.x) - 2, ViewUtils::vy(nodePos.y) - 1, symbol);
}

void ViewBoard::clearLines(const Position& nodePos) const
{
    m_ncurses.print(ViewUtils::vx(nodePos.x), ViewUtils::vy(nodePos.y) - 1, " ");
    m_ncurses.print(ViewUtils::vx(nodePos.x) + 1, ViewUtils::vy(nodePos.y), "  ");
    m_ncurses.print(ViewUtils::vx(nodePos.x) + 1, ViewUtils::vy(nodePos.y) - 1, "  ");
}

void ViewBoard::drawMarker(const Position& nodePos, MarkerVisability visability) const
{
    if (visability == MarkerVisability::Occupied) {
        m_ncurses.print(ViewUtils::vx(nodePos.x), ViewUtils::vy(nodePos.y), "+");
    } else if (visability == MarkerVisability::NotOccupied) {
        m_ncurses.print(ViewUtils::vx(nodePos.x), ViewUtils::vy(nodePos.y), "+", ColorPair::MARK_GRAY);
    }
}

void ViewBoard::drawNames(const std::string& topName, ColorPair topColor,
    const std::string& bottomName, ColorPair bottomColor) const
{
    auto x = ViewUtils::vx(m_board.getGoalpostRight()) + 2;
    auto y = ViewUtils::Y_OFFSET + 1;
    m_ncurses.print(x, y, topName, topColor);

    y = ViewUtils::vy(m_board.getHeight() - 1) - 1;
    m_ncurses.print(x, y, bottomName, bottomColor);
}

void ViewBoard::drawPathMarkers(const std::vector<Direction>& dirPath, ColorPair ballColor) const
{
    auto nodePos = m_board.getBallPosition();
    m_ncurses.print(ViewUtils::vx(nodePos.x), ViewUtils::vy(nodePos.y), "*", ballColor);

    for (auto dir : std::ranges::views::reverse(dirPath)) {
        auto reverseDir = reverseDirection(dir);
        nodePos = directionToPosition(nodePos, reverseDir);
        m_ncurses.print(ViewUtils::vx(nodePos.x), ViewUtils::vy(nodePos.y), "*", ballColor);
    }
}

} // namespace PaperSoccer
