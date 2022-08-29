// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#include "ViewMenu.hpp"
#include <iomanip>
#include <sstream>

namespace PaperSoccer {

namespace {
    int vx(int x)
    {
        return x * ViewMenu::X_FACTOR + ViewMenu::X_OFFSET;
    }
} // namespace anonymous

ViewMenu::ViewMenu(const IBoard& board, const INCurses& ncurses)
    : m_board{board}
    , m_ncurses{ncurses}
{
}

void ViewMenu::drawLegend(char undo, char newGame, const std::map<char, Direction>& dirKeys) const
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

void ViewMenu::setContinueStatus() const
{
    drawStatusButton("  Your turn.  ", "  Make move   ", ColorPair::BUTTON_GREEN);
}

void ViewMenu::setEnemyTurnStatus() const
{
    drawStatusButton("  Enemy turn. ", "     Wait     ", ColorPair::BUTTON_GRAY);
}

void ViewMenu::setReadyToEndTurnStatus() const
{
    drawStatusButton("  End turn.   ", "   (Enter)    ", ColorPair::BUTTON_YELLOW);
}

void ViewMenu::setLostStatus(int wins, int lost) const
{
    drawScore(wins, lost);
    drawStatusButton("   You Lost.  ", " New game (n) ", ColorPair::BUTTON_ORANGE);
}

void ViewMenu::setWinStatus(int won, int lost) const
{
    drawScore(won, lost);
    drawStatusButton("   You Win.   ", " New game (n) ", ColorPair::BUTTON_ORANGE);
}

void ViewMenu::drawStatusButton(const std::string& line1, const std::string& line2, ColorPair color) const
{
    const auto x = getMenuXOffset();
    const auto y = Y_OFFSET;
    m_ncurses.print(x, y + 0, TOP_LINE, color);
    m_ncurses.print(x, y + 1, "|" + line1 + "|", color);
    m_ncurses.print(x, y + 2, "|" + line2 + "|", color);
    m_ncurses.print(x, y + 3, BOTTOM_LINE, color);

    m_ncurses.refreshView();
}

unsigned int ViewMenu::getMenuXOffset() const
{
    constexpr auto space{2};
    return vx(m_board.getWidth()) + space;
}

void ViewMenu::drawScore(int won, int lost) const
{
    const auto x = getMenuXOffset();
    const auto y = Y_SCORE_OFFSET;

    m_ncurses.print(x, y + 0, " Score:");
    m_ncurses.print(x, y + 1, "   Won: " + std::to_string(won), ColorPair::USER);
    m_ncurses.print(x, y + 2, "  Lost: " + std::to_string(lost), ColorPair::ENEMY);
}

void ViewMenu::drawTimeLeft(std::chrono::milliseconds userTimeLeft, std::chrono::milliseconds enemyTimeLeft) const
{
    const auto x = getMenuXOffset();
    const auto y = Y_TIME_OFFSET;

    m_ncurses.print(x, y, " Time left:");

    drawUserTimeLeft(userTimeLeft);
    drawEnemyTimeLeft(enemyTimeLeft);
}

void ViewMenu::drawUserTimeLeft(std::chrono::milliseconds timeLeft) const
{
    const auto x = getMenuXOffset();
    const auto y = Y_TIME_OFFSET;

    drawTime(x, y + 1, timeLeft, "     Me: ", ColorPair::USER);
}

void ViewMenu::drawEnemyTimeLeft(std::chrono::milliseconds timeLeft) const
{
    const auto x = getMenuXOffset();
    const auto y = Y_TIME_OFFSET;

    drawTime(x, y + 2, timeLeft, "  Enemy: ", ColorPair::ENEMY);
}

void ViewMenu::drawTime(unsigned int x, unsigned int y, std::chrono::milliseconds timeLeft, const std::string& name, ColorPair color) const
{
    auto minutes = duration_cast<std::chrono::minutes>(timeLeft);
    auto seconds = duration_cast<std::chrono::seconds>(timeLeft - minutes);
    std::ostringstream ss;
    ss << std::setw(2) << std::setfill('0') << minutes.count();
    ss << ":" << std::setw(2) << std::setfill('0') << seconds.count();

    m_ncurses.print(x, y, name + ss.str(), color);
    m_ncurses.refreshView();
}

bool ViewMenu::isStatusButton(unsigned int x, unsigned int y) const
{
    const auto buttonX = getMenuXOffset();
    const auto buttonY = Y_OFFSET;
    return x >= buttonX and x <= TOP_LINE.size() + buttonX and y >= buttonY and y <= buttonY + BUTTON_HEIGHT;
}

} // namespace PaperSoccer
