// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#include "View.hpp"
#include <iomanip>
#include <sstream>

namespace PaperSoccer {

namespace {
    int vx(int x)
    {
        return x * View::X_FACTOR + View::X_OFFSET;
    }

    int vy(int y)
    {
        return y * View::Y_FACTOR + View::Y_OFFSET;
    }

} // namespace anonymous

View::View(const IBoard& board, const INCurses& ncurses)
    : m_board{board}
    , m_ncurses{ncurses}
{
}

void View::clear() const
{
    m_ncurses.clearView();
}

void View::drawBoard(const std::string& topName, ColorPair topColor, const std::string& bottomName, ColorPair bottomColor,
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

Skips View::filterDirsForTopBorderLine(Position nodePos) const
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

Skips View::filterDirsForBottomNetLine(Position nodePos) const
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

Skips View::filterDirsForRightLine(Position nodePos) const
{
    const int rightLine = m_board.getWidth() - 1;

    if (nodePos.x == rightLine) {
        return Skips{Skip{Direction::TopRight, Direction::Right}, Skip{}};
    }

    return Skips{};
}

MarkerVisability View::markerVisability(Position nodePos) const
{
    const int bottomLine = m_board.getHeight() - 1;

    if ((nodePos.y == 0 or nodePos.y == bottomLine) and m_board.hasAllNeighbours(nodePos)) {
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

void View::drawVerticalToTopLine(const Position& nodePos) const
{
    m_ncurses.print(vx(nodePos.x), vy(nodePos.y) - 1, "|");
}

void View::drawHorizontalToRightLine(const Position& nodePos) const
{
    m_ncurses.print(vx(nodePos.x) + 1, vy(nodePos.y), "--");
}

void View::drawCrossToRight(const Position& nodePos) const
{
    // >< - U+003e U+003f https://en.wikipedia.org/wiki/Basic_Latin_(Unicode_block)
    // ᐳᐸ - U+1433 U+1438 https://en.wikipedia.org/wiki/Unified_Canadian_Aboriginal_Syllabics_(Unicode_block)
    const std::string symbol = "ᐳᐸ";
    m_ncurses.print(vx(nodePos.x) + 1, vy(nodePos.y) - 1, symbol);
}

void View::drawHypotenuseToTopRight(const Position& nodePos) const
{
    //  / - U+002F        https://en.wikipedia.org/wiki/Basic_Latin_(Unicode_block)
    // ⸝⸍ - U+2e1d U+2e0d https://en.wikipedia.org/wiki/Supplemental_Punctuation
    // ᐟ  - U+141f        https://en.wikipedia.org/wiki/Unified_Canadian_Aboriginal_Syllabics_(Unicode_block)
    // ৴  - U+09f4        https://en.wikipedia.org/wiki/Bengali_(Unicode_block)
    const std::string symbol = "⸝⸍";
    m_ncurses.print(vx(nodePos.x) + 1, vy(nodePos.y) - 1, symbol);
}

void View::drawHypotenuseToTopLeft(const Position& nodePos) const
{
    //  \ - U+005C        https://en.wikipedia.org/wiki/Basic_Latin_(Unicode_block)
    // ⸌⸜ - U+2e0c U+2e1c https://en.wikipedia.org/wiki/Supplemental_Punctuation
    // ᐠ  - U+1420        https://en.wikipedia.org/wiki/Unified_Canadian_Aboriginal_Syllabics_(Unicode_block)
    // ৲  - U+09f2        https://en.wikipedia.org/wiki/Bengali_(Unicode_block)
    // ヽ  - U+30FD       https://en.wikipedia.org/wiki/Katakana_(Unicode_block)
    const std::string symbol = "⸌⸜";
    m_ncurses.print(vx(nodePos.x) - 2, vy(nodePos.y) - 1, symbol);
}

void View::clearLines(const Position& nodePos) const
{
    m_ncurses.print(vx(nodePos.x), vy(nodePos.y) - 1, " ");
    m_ncurses.print(vx(nodePos.x) + 1, vy(nodePos.y), "  ");
    m_ncurses.print(vx(nodePos.x) + 1, vy(nodePos.y) - 1, "  ");
}

void View::drawMarker(const Position& nodePos, MarkerVisability visability) const
{
    if (visability == MarkerVisability::Occupied) {
        m_ncurses.print(vx(nodePos.x), vy(nodePos.y), "+");
    } else if (visability == MarkerVisability::NotOccupied) {
        m_ncurses.print(vx(nodePos.x), vy(nodePos.y), "+", ColorPair::MARK_GRAY);
    }
}

void View::drawLegend(char undo, char newGame, const std::map<char, Direction>& dirKeys) const
{
    auto x = getMenuXOffset();
    auto y = Y_LEGEND_OFFSET;

    constexpr auto descShift{1};
    m_ncurses.print(x + descShift, y + 0, "Keys:");
    m_ncurses.print(x + descShift, y + 1, " Undo (" + std::string{undo} + ")");
    m_ncurses.print(x + descShift, y + 2, " End turn (Enter)");
    m_ncurses.print(x + descShift, y + 3, " New game (" + std::string{newGame} + ")");

    m_ncurses.print(x + descShift, y + 5, "Directions keys:");

    // Set at center of arrows
    x += 7;
    y += 8;

    // https://en.wikipedia.org/wiki/Arrows_(Unicode_block)
    constexpr auto xArrOffset{2};
    constexpr auto yArrOffset{1};
    m_ncurses.print(x - xArrOffset, y - yArrOffset, "↖");
    m_ncurses.print(x, y - yArrOffset, "↑");
    m_ncurses.print(x + xArrOffset, y - yArrOffset, "↗");

    m_ncurses.print(x - xArrOffset, y, "←");
    m_ncurses.print(x + xArrOffset, y, "→");

    m_ncurses.print(x - xArrOffset, y + yArrOffset, "↙");
    m_ncurses.print(x, y + yArrOffset, "↓");
    m_ncurses.print(x + xArrOffset, y + yArrOffset, "↘");

    constexpr auto xKeyOffset{4};
    constexpr auto yKeyOffset{2};
    for (auto [key, dir] : dirKeys) {
        switch (dir) {
        case Direction::TopLeft:
            m_ncurses.print(x - xKeyOffset, y - yKeyOffset, std::string{key});
            break;
        case Direction::Top:
            m_ncurses.print(x, y - yKeyOffset, std::string{key});
            break;
        case Direction::TopRight:
            m_ncurses.print(x + xKeyOffset, y - yKeyOffset, std::string{key});
            break;
        case Direction::Left:
            m_ncurses.print(x - xKeyOffset, y, std::string{key});
            break;
        case Direction::Right:
            m_ncurses.print(x + xKeyOffset, y, std::string{key});
            break;
        case Direction::BottomLeft:
            m_ncurses.print(x - xKeyOffset, y + yKeyOffset, std::string{key});
            break;
        case Direction::Bottom:
            m_ncurses.print(x, y + yKeyOffset, std::string{key});
            break;
        case Direction::BottomRight:
            m_ncurses.print(x + xKeyOffset, y + yKeyOffset, std::string{key});
            break;
        default:
            break;
        }
    }
}

void View::drawNames(const std::string& topName, ColorPair topColor,
    const std::string& bottomName, ColorPair bottomColor) const
{
    auto x = vx(m_board.getGoalpostRight()) + 2;
    auto y = Y_OFFSET + 1;
    m_ncurses.print(x, y, topName, topColor);

    y = vy(m_board.getHeight() - 1) - 1;
    m_ncurses.print(x, y, bottomName, bottomColor);
}

void View::drawPathMarkers(const std::vector<Direction>& dirPath, ColorPair ballColor) const
{
    auto nodePos = m_board.getBallPosition();
    m_ncurses.print(vx(nodePos.x), vy(nodePos.y), "*", ballColor);

    for (auto dir : std::ranges::views::reverse(dirPath)) {
        auto reverseDir = reverseDirection(dir);
        nodePos = directionToPosition(nodePos, reverseDir);
        m_ncurses.print(vx(nodePos.x), vy(nodePos.y), "*", ballColor);
    }
}

void View::setContinueStatus() const
{
    drawStatusButton("  Your turn.  ", "  Make move   ", ColorPair::BUTTON_GREEN);
}

void View::setEnemyTurnStatus() const
{
    drawStatusButton("  Enemy turn. ", "     Wait     ", ColorPair::BUTTON_GRAY);
}

void View::setReadyToEndTurnStatus() const
{
    drawStatusButton("  End turn.   ", "   (Enter)    ", ColorPair::BUTTON_YELLOW);
}

void View::setLostStatus(int wins, int lost) const
{
    drawScore(wins, lost);
    drawStatusButton("   You Lost.  ", " New game (n) ", ColorPair::BUTTON_ORANGE);
}

void View::setWinStatus(int won, int lost) const
{
    drawScore(won, lost);
    drawStatusButton("   You Win.   ", " New game (n) ", ColorPair::BUTTON_ORANGE);
}

void View::drawStatusButton(const std::string& line1, const std::string& line2, ColorPair color) const
{
    const auto x = getMenuXOffset();
    const auto y = Y_OFFSET;
    m_ncurses.print(x, y + 0, TOP_LINE, color);
    m_ncurses.print(x, y + 1, "|" + line1 + "|", color);
    m_ncurses.print(x, y + 2, "|" + line2 + "|", color);
    m_ncurses.print(x, y + 3, BOTTOM_LINE, color);

    m_ncurses.refreshView();
}

unsigned int View::getMenuXOffset() const
{
    constexpr auto space{2};
    return vx(m_board.getWidth()) + space;
}

void View::drawScore(int won, int lost) const
{
    const auto x = getMenuXOffset();
    const auto y = Y_SCORE_OFFSET;

    m_ncurses.print(x, y + 0, " Score:");
    m_ncurses.print(x, y + 1, "   Won: " + std::to_string(won), ColorPair::USER);
    m_ncurses.print(x, y + 2, "  Lost: " + std::to_string(lost), ColorPair::ENEMY);
}

void View::drawTimeLeft(std::chrono::milliseconds userTimeLeft, std::chrono::milliseconds enemyTimeLeft) const
{
    const auto x = getMenuXOffset();
    const auto y = Y_TIME_OFFSET;

    m_ncurses.print(x, y, " Time left:");

    drawUserTimeLeft(userTimeLeft);
    drawEnemyTimeLeft(enemyTimeLeft);
}

void View::drawUserTimeLeft(std::chrono::milliseconds timeLeft) const
{
    const auto x = getMenuXOffset();
    const auto y = Y_TIME_OFFSET;

    drawTime(x, y + 1, timeLeft, "     Me: ", ColorPair::USER);
}

void View::drawEnemyTimeLeft(std::chrono::milliseconds timeLeft) const
{
    const auto x = getMenuXOffset();
    const auto y = Y_TIME_OFFSET;

    drawTime(x, y + 2, timeLeft, "  Enemy: ", ColorPair::ENEMY);
}

void View::drawTime(unsigned int x, unsigned int y, std::chrono::milliseconds timeLeft, const std::string& name, ColorPair color) const
{
    auto minutes = duration_cast<std::chrono::minutes>(timeLeft);
    auto seconds = duration_cast<std::chrono::seconds>(timeLeft - minutes);
    std::ostringstream ss;
    ss << std::setw(2) << std::setfill('0') << minutes.count();
    ss << ":" << std::setw(2) << std::setfill('0') << seconds.count();

    m_ncurses.print(x, y, name + ss.str(), color);
    m_ncurses.refreshView();
}

bool View::isStatusButton(unsigned int x, unsigned int y) const
{
    const auto buttonX = getMenuXOffset();
    const auto buttonY = Y_OFFSET;
    return x >= buttonX and x <= TOP_LINE.size() + buttonX and y >= buttonY and y <= buttonY + BUTTON_HEIGHT;
}

std::optional<Direction> View::getMoveDirection(int x, int y) const
{
    const auto ballPos = m_board.getBallPosition();
    const auto viewX = vx(ballPos.x);
    const auto viewY = vy(ballPos.y);

    if (x == viewX - X_FACTOR) {
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
