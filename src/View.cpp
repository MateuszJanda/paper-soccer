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

View::View(const IBoard& board, const INCurses& ncurses, const IViewBoard &viewBoard)
    : m_board{board}
    , m_ncurses{ncurses}
    , m_viewBoard{viewBoard}
{
}

void View::clear() const
{
    m_ncurses.clearView();
}

void View::drawBoard(const std::string& topName, ColorPair topColor, const std::string& bottomName, ColorPair bottomColor,
    const std::vector<Direction>& dirPath, ColorPair ballColor) const
{
    m_viewBoard.drawBoard(topName, topColor, bottomName, bottomColor, dirPath, ballColor);
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
