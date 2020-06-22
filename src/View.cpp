// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#include "View.hpp"
#include "Board.hpp"
#include "Direction.hpp"
#include "Position.hpp"
#include <set>

namespace PaperSoccer {

View::View(const IBoard &board, const INCurses &ncurses)
    : m_board{board}
    , m_ncurses{ncurses}
{
}

void View::clear() const
{
    m_ncurses.clearView();
}

void View::drawBoard(std::string topName, std::string bottomName, std::vector<Direction> dirPath) const
{
    for (auto y = 0; y < m_board.getHeight(); y++) {
        for (auto x = 0; x < m_board.getWidth(); x++) {
            Position nodePos{x, y};
            auto [nodeSkip, neighSkip] = filterDirsForOutOfBorder(nodePos);
            auto visability = markerVisability(nodePos);
            drawCell(nodePos, nodeSkip, neighSkip, visability);
        }
    }

    drawNames(topName, bottomName);
    drawPathMarkers(dirPath);
    m_ncurses.refreshView();
}

Skips View::filterDirsForOutOfBorder(Position nodePos) const
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

Skips View::filterDirsForTopNetLine(Position nodePos) const
{
    const std::size_t topNetLine = 0;

    if (nodePos.y == topNetLine) {
        if (nodePos.x >= m_board.getGoalpostLeft() and nodePos.x < m_board.getGoalpostRight()) {
            return Skips{Skip{Direction::Top, Direction::TopRight}, Skip{Direction::TopLeft}};
        } else {
            return Skips{Skip{Direction::Top, Direction::TopRight, Direction::Right},
                Skip{Direction::TopLeft}};
        }
    }

    return Skips{};
}

Skips View::filterDirsForTopBorderLine(Position nodePos) const
{
    const std::size_t topBorderLine = 1;

    if (nodePos.y == topBorderLine) {
        if (nodePos.x < m_board.getGoalpostLeft() or nodePos.x > m_board.getGoalpostRight()) {
            return Skips{Skip{Direction::Top, Direction::TopRight}, Skip{Direction::TopLeft}};
        } else if (nodePos.x == m_board.getGoalpostRight()) {
            return Skips{Skip{Direction::TopRight}, Skip{Direction::TopLeft}};
        }
    }

    return Skips{};
}

Skips View::filterDirsForBottomNetLine(Position nodePos) const
{
    const std::size_t bottomNetLine = m_board.getHeight() - 1;

    if (nodePos.y == bottomNetLine) {
        if (nodePos.x < m_board.getGoalpostLeft() or nodePos.x > m_board.getGoalpostRight()) {
            return Skips{Skip{Direction::Top, Direction::TopRight, Direction::Right},
                Skip{Direction::TopLeft}};
        } else if (nodePos.x == m_board.getGoalpostRight()) {
            return Skips{Skip{Direction::TopRight, Direction::Right}, Skip{Direction::TopLeft}};
        }
    }

    return Skips{};
}

Skips View::filterDirsForRightLine(Position nodePos) const
{
    const std::size_t rightLine = m_board.getWidth() - 1;

    if (nodePos.x == rightLine) {
        return Skips{Skip{Direction::TopRight, Direction::Right}, Skip{}};
    }

    return Skips{};
}

MarkerVisability View::markerVisability(Position nodePos) const
{
    if ((nodePos.y == 0 or nodePos.y == m_board.getHeight() - 1) and m_board.hasAllNeighbours(nodePos)) {
        return MarkerVisability::Invisible;
    } else if (m_board.hasAnyNeighbour(nodePos)) {
        return MarkerVisability::Occupied;
    }

    return MarkerVisability::NotOccupied;
}

void View::drawCell(Position nodePos, Skip nodeSkip, Skip neighSkip, MarkerVisability visability) const
{
    clearLines(nodePos);

    if (not nodeSkip.contains(Direction::Top) and m_board.hasNeighbour(nodePos, Direction::Top)) {
        drawVerticalToTopLine(nodePos);
    }

    if (not nodeSkip.contains(Direction::Right) and m_board.hasNeighbour(nodePos, Direction::Right)) {
        drawHorizontalToRightLine(nodePos);
    }

    bool topRight = false;
    if (not nodeSkip.contains(Direction::TopRight) and m_board.hasNeighbour(nodePos, Direction::TopRight)) {
        topRight = true;
    }

    bool topLeft = false;
    const Position neighbourPos{nodePos.x + 1, nodePos.y};
    if (not neighSkip.contains(Direction::TopLeft) and neighbourPos.x < m_board.getWidth()
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

void View::drawVerticalToTopLine(Position nodePos) const
{
    m_ncurses.print(nodePos.x * X_FACTOR + X_OFFSET, nodePos.y * Y_FACTOR - 1 + Y_OFFSET, "|");
}

void View::drawHorizontalToRightLine(Position nodePos) const
{
    m_ncurses.print(nodePos.x * X_FACTOR + 1 + X_OFFSET, nodePos.y * Y_FACTOR + Y_OFFSET, "--");
}

void View::drawCrossToRight(Position nodePos) const
{
    // >< - U+003e U+003f https://en.wikipedia.org/wiki/Basic_Latin_(Unicode_block)
    // ᐳᐸ - U+1433 U+1438 https://en.wikipedia.org/wiki/Unified_Canadian_Aboriginal_Syllabics_(Unicode_block)
    const std::string symbol = "ᐳᐸ";
    m_ncurses.print(nodePos.x * X_FACTOR + 1 + X_OFFSET, nodePos.y * Y_FACTOR - 1 + Y_OFFSET, symbol);
}

void View::drawHypotenuseToTopRight(Position nodePos) const
{
    //  / - U+002F         https://en.wikipedia.org/wiki/Basic_Latin_(Unicode_block)
    // ⸝⸍ - U+2e1d U+2e0d https://en.wikipedia.org/wiki/Supplemental_Punctuation
    // ᐟ  - U+141f        https://en.wikipedia.org/wiki/Unified_Canadian_Aboriginal_Syllabics_(Unicode_block)
    // ৴  - U+09f4        https://en.wikipedia.org/wiki/Bengali_(Unicode_block)
    const std::string symbol = "⸝⸍";
    m_ncurses.print(nodePos.x * X_FACTOR + 1 + X_OFFSET, nodePos.y * Y_FACTOR - 1 + Y_OFFSET, symbol);
}

void View::drawHypotenuseToTopLeft(Position nodePos) const
{
    //  \ - U+005C         https://en.wikipedia.org/wiki/Basic_Latin_(Unicode_block)
    // ⸌⸜ - U+2e0c U+2e1c https://en.wikipedia.org/wiki/Supplemental_Punctuation
    // ᐠ  - U+1420        https://en.wikipedia.org/wiki/Unified_Canadian_Aboriginal_Syllabics_(Unicode_block)
    // ৲  - U+09f2        https://en.wikipedia.org/wiki/Bengali_(Unicode_block)
    // ヽ  - U+30FD       https://en.wikipedia.org/wiki/Katakana_(Unicode_block)
    const std::string symbol = "⸌⸜";
    m_ncurses.print(nodePos.x * X_FACTOR - 2 + X_OFFSET, nodePos.y * Y_FACTOR - 1 + Y_OFFSET, symbol);
}

void View::clearLines(Position nodePos) const
{
    m_ncurses.print(nodePos.x * X_FACTOR + X_OFFSET, nodePos.y * Y_FACTOR - 1 + Y_OFFSET, " ");
    m_ncurses.print(nodePos.x * X_FACTOR + 1 + X_OFFSET, nodePos.y * Y_FACTOR + Y_OFFSET, "  ");
    m_ncurses.print(nodePos.x * X_FACTOR + 1 + X_OFFSET, nodePos.y * Y_FACTOR - 1 + Y_OFFSET, "  ");
}

void View::drawMarker(Position nodePos, MarkerVisability visability) const
{
    if (visability == MarkerVisability::Occupied) {
        m_ncurses.print(nodePos.x * X_FACTOR + X_OFFSET, nodePos.y * Y_FACTOR + Y_OFFSET, "+");
    } else if (visability == MarkerVisability::NotOccupied) {
        m_ncurses.print(nodePos.x * X_FACTOR + X_OFFSET, nodePos.y * Y_FACTOR + Y_OFFSET, ".");
    }
}

void View::drawLegend(char undo, char newGame, std::map<char, Direction> dirKeys) const
{
    auto x = getStatusButtonXShift();
    auto y = Y_OFFSET + 3 + 5;

    m_ncurses.print(x, y, "Undo: " + std::string{undo});
    m_ncurses.print(x, y + 1, "New game: " + std::string{newGame});

    // Set at center of arrows
    x += 4;
    y += 4;

    // https://en.wikipedia.org/wiki/Arrows_(Unicode_block)
    m_ncurses.print(x - 2, y - 1, "↖");
    m_ncurses.print(x, y - 1, "↑");
    m_ncurses.print(x + 2, y - 1, "↗");

    m_ncurses.print(x - 2, y, "←");
    m_ncurses.print(x + 2, y, "→");

    m_ncurses.print(x - 2, y + 1, "↙");
    m_ncurses.print(x, y + 1, "↓");
    m_ncurses.print(x + 2, y + 1, "↘");

    for (auto [key, dir] : dirKeys) {
        switch (dir) {
        case Direction::TopLeft:
            m_ncurses.print(x - 4, y - 2, std::string{key});
            break;
        case Direction::Top:
            m_ncurses.print(x, y - 2, std::string{key});
            break;
        case Direction::TopRight:
            m_ncurses.print(x + 4, y - 2, std::string{key});
            break;
        case Direction::Left:
            m_ncurses.print(x - 4, y, std::string{key});
            break;
        case Direction::Right:
            m_ncurses.print(x + 4, y, std::string{key});
            break;
        case Direction::BottomLeft:
            m_ncurses.print(x - 4, y + 2, std::string{key});
            break;
        case Direction::Bottom:
            m_ncurses.print(x, y + 2, std::string{key});
            break;
        case Direction::BottomRight:
            m_ncurses.print(x + 4, y + 2, std::string{key});
            break;
        default:
            break;
        }
    }
}

void View::drawNames(std::string topName, std::string bottomName) const
{
    auto x = m_board.getGoalpostRight() * X_FACTOR + 2 + X_OFFSET;
    auto y = 1 + Y_OFFSET;
    m_ncurses.print(x, y, topName);

    y = (m_board.getHeight() - 1) * Y_FACTOR - 1 + Y_OFFSET;
    m_ncurses.print(x, y, bottomName);
}

void View::drawPathMarkers(std::vector<Direction> dirPath) const
{
    Position nodePos = m_board.getBallPosition();
    m_ncurses.print(nodePos.x * X_FACTOR + X_OFFSET, nodePos.y * Y_FACTOR + Y_OFFSET, "*");

    // TODO: Supported in gcc 10
    // for (auto v : std::ranges::views::reverse(dirPath))
    for (auto it = dirPath.rbegin(); it != dirPath.rend(); ++it) {
        auto reverseDir = reverseDirection(*it);
        nodePos = directionToPosition(nodePos, reverseDir);
        m_ncurses.print(nodePos.x * X_FACTOR + X_OFFSET, nodePos.y * Y_FACTOR + Y_OFFSET, "*");
    }
}

void View::setContinueStatus() const
{
    drawStatusButton("  Your turn.  ", "  Make move   ");
}

void View::setEnemyTurnStatus() const
{
    drawStatusButton("  Enemy turn. ", "     Wait     ");
}

void View::setReadyToEndTurnStatus() const
{
    drawStatusButton("  End turn.   ", "   (Enter)    ");
}

void View::setLostStatus(int wins, int lost) const
{
    drawScore(wins, lost);
    drawStatusButton("   You Lost.  ", " New game (n) ");
}

void View::setWinStatus(int won, int lost) const
{
    drawScore(won, lost);
    drawStatusButton("   You Win.   ", " New game (n) ");
}

void View::drawStatusButton(std::string line1, std::string line2) const
{
    const int x = getStatusButtonXShift();
    const int y = Y_OFFSET;
    m_ncurses.print(x, y + 0, LINE);
    m_ncurses.print(x, y + 1, "|" + line1 + "|");
    m_ncurses.print(x, y + 2, "|" + line2 + "|");
    m_ncurses.print(x, y + 3, LINE);

    m_ncurses.refreshView();
}

int View::getStatusButtonXShift() const
{
    return X_OFFSET + m_board.getWidth() * X_FACTOR + 2;
}

void View::drawScore(int won, int lost) const
{
    auto x = getStatusButtonXShift();
    auto y = Y_OFFSET + 3 + 2;

    m_ncurses.print(x, y + 0, "   Won: " + std::to_string(won), ColorPair::USER);
    m_ncurses.print(x, y + 1, "  Lost: " + std::to_string(lost), ColorPair::ENEMY);
}

bool View::isStatusButton(int x, int y) const
{
    const auto buttonX = getStatusButtonXShift();
    const auto buttonY = Y_OFFSET;
    return x >= buttonX and x <= LINE.size() + buttonX and y >= buttonY and y <= buttonY + 3;
}

std::optional<Direction> View::getMoveDirection(int x, int y) const
{
    const auto ballPos = m_board.getBallPosition();
    const auto viewX = ballPos.x * X_FACTOR + X_OFFSET;
    const auto viewY = ballPos.y * Y_FACTOR + Y_OFFSET;

    if (x == viewX - X_FACTOR)
    {
        if (y == viewY - Y_FACTOR) {
            return Direction::TopLeft;
        } else if (y == viewY) {
            return Direction::Left;
        } else if (y == viewY + Y_FACTOR) {
            return Direction::BottomLeft;
        }
    } else if (x == viewX) {
        if (y == viewY - Y_FACTOR) {
            return Direction::Top;
        } else if (y == viewY + Y_FACTOR) {
            return Direction::Bottom;
        }
    } else if (x == viewX + X_FACTOR) {
        if (y == viewY - Y_FACTOR) {
            return Direction::TopRight;
        } else if (y == viewY) {
            return Direction::Right;
        } else if (y == viewY + Y_FACTOR) {
            return Direction::BottomRight;
        }
    }

    return std::nullopt;
}

} // namespace PaperSoccer
