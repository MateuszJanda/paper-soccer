#include "View.hpp"
#include "Board.hpp"
#include "Direction.hpp"
#include "Position.hpp"
#include <set>

namespace PaperSoccer {

View::View(IBoard& board, INCurses& ncurses)
    : m_board{board}
    , m_ncurses{ncurses}
{
}

void View::drawBoard()
{
    for (auto y = 0; y < m_board.getHeight(); y++) {
        for (auto x = 0; x < m_board.getWidth(); x++) {
            Position nodePos{x, y};
            auto [nodeSkip, neighSkip] = filterDirsForOutOfBorder(nodePos);
            drawCell(nodePos, nodeSkip, neighSkip);
        }
    }

    m_ncurses.refreshView();
}

Skips View::filterDirsForOutOfBorder(Position nodePos)
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

Skips View::filterDirsForTopNetLine(Position nodePos)
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

Skips View::filterDirsForTopBorderLine(Position nodePos)
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

Skips View::filterDirsForBottomNetLine(Position nodePos)
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

Skips View::filterDirsForRightLine(Position nodePos)
{
    const std::size_t rightLine = m_board.getWidth() - 1;

    if (nodePos.x == rightLine) {
        return Skips{Skip{Direction::TopRight, Direction::Right}, Skip{}};
    }

    return Skips{};
}

void View::drawCell(Position nodePos, Skip nodeSkip, Skip neighSkip)
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

    drawMarker(nodePos);
}

void View::drawVerticalToTopLine(Position nodePos)
{
    m_ncurses.print(nodePos.x * X_FACTOR + X_OFFSET, nodePos.y * Y_FACTOR - 1 + Y_OFFSET, "|");
}

void View::drawHorizontalToRightLine(Position nodePos)
{
    m_ncurses.print(nodePos.x * X_FACTOR + 1 + X_OFFSET, nodePos.y * Y_FACTOR + Y_OFFSET, "--");
}

void View::drawCrossToRight(Position nodePos)
{
    // >< - U+003e U+003f https://en.wikipedia.org/wiki/Basic_Latin_(Unicode_block)
    // ᐳᐸ - U+1433 U+1438 https://en.wikipedia.org/wiki/Unified_Canadian_Aboriginal_Syllabics_(Unicode_block)
    const std::string symbol = "ᐳᐸ";
    m_ncurses.print(nodePos.x * X_FACTOR + 1 + X_OFFSET, nodePos.y * Y_FACTOR - 1 + Y_OFFSET, symbol);
}

void View::drawHypotenuseToTopRight(Position nodePos)
{
    //  / - U+002F         https://en.wikipedia.org/wiki/Basic_Latin_(Unicode_block)
    // ⸝⸍ - U+2e1d U+2e0d https://en.wikipedia.org/wiki/Supplemental_Punctuation
    // ᐟ  - U+141f        https://en.wikipedia.org/wiki/Unified_Canadian_Aboriginal_Syllabics_(Unicode_block)
    // ৴  - U+09f4        https://en.wikipedia.org/wiki/Bengali_(Unicode_block)
    const std::string symbol = "⸝⸍";
    m_ncurses.print(nodePos.x * X_FACTOR + 1 + X_OFFSET, nodePos.y * Y_FACTOR - 1 + Y_OFFSET, symbol);
}

void View::drawHypotenuseToTopLeft(Position nodePos)
{
    //  \ - U+005C         https://en.wikipedia.org/wiki/Basic_Latin_(Unicode_block)
    // ⸌⸜ - U+2e0c U+2e1c https://en.wikipedia.org/wiki/Supplemental_Punctuation
    // ᐠ  - U+1420        https://en.wikipedia.org/wiki/Unified_Canadian_Aboriginal_Syllabics_(Unicode_block)
    // ৲  - U+09f2        https://en.wikipedia.org/wiki/Bengali_(Unicode_block)
    // ヽ  - U+30FD       https://en.wikipedia.org/wiki/Katakana_(Unicode_block)
    const std::string symbol = "⸌⸜";
    m_ncurses.print(nodePos.x * X_FACTOR - 2 + X_OFFSET, nodePos.y * Y_FACTOR - 1 + Y_OFFSET, symbol);
}

void View::clearLines(Position nodePos)
{
    m_ncurses.print(nodePos.x * X_FACTOR + X_OFFSET, nodePos.y * Y_FACTOR - 1 + Y_OFFSET, " ");
    m_ncurses.print(nodePos.x * X_FACTOR + 1 + X_OFFSET, nodePos.y * Y_FACTOR + Y_OFFSET, "  ");
    m_ncurses.print(nodePos.x * X_FACTOR + 1 + X_OFFSET, nodePos.y * Y_FACTOR - 1 + Y_OFFSET, "  ");
}

void View::drawMarker(Position nodePos)
{
    m_ncurses.print(nodePos.x * X_FACTOR + X_OFFSET, nodePos.y * Y_FACTOR + Y_OFFSET, "+");
}

void View::printText(int x, int y, std::string str)
{
    m_ncurses.print(x, y, str);
}

void View::setContinueStatus()
{
    drawStatus("  Your turn.  ", "  Make move   ");
}

void View::setEnemyTurnStatus()
{
    drawStatus("  Enemy turn. ", "     Wait     ");
}

void View::setReadyToEndTurnStatus()
{
    drawStatus("  End turn.   ", " Press Enter  ");
}

void View::setLostStatus()
{
    drawStatus("   You Lost.  ", " New game (n) ");
}

void View::setWinStatus()
{
    drawStatus("   You Win.   ", " New game (n) ");
}

void View::drawStatus(std::string line1, std::string line2)
{
    const int x = X_OFFSET + m_board.getWidth() * X_FACTOR + 2;
    const int y = Y_OFFSET;
    m_ncurses.print(x, y + 0, "+--------------+");
    m_ncurses.print(x, y + 1, "|" + line1 + "|");
    m_ncurses.print(x, y + 2, "|" + line2 + "|");
    m_ncurses.print(x, y + 3, "+--------------+");

    m_ncurses.refreshView();
}

} // namespace PaperSoccer
