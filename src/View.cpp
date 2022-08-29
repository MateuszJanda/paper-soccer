// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#include "View.hpp"
#include <iomanip>
#include <sstream>

namespace PaperSoccer {

View::View(const IBoard& board, const INCurses& ncurses, const IViewBoard& viewBoard, const IViewMenu& viewMenu)
    : m_board{board}
    , m_ncurses{ncurses}
    , m_viewBoard{viewBoard}
    , m_viewMenu{viewMenu}
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
    m_viewMenu.drawLegend(undo, newGame, dirKeys);
}

void View::setContinueStatus() const
{
    m_viewMenu.setContinueStatus();
}

void View::setEnemyTurnStatus() const
{
    m_viewMenu.setEnemyTurnStatus();
}

void View::setReadyToEndTurnStatus() const
{
    m_viewMenu.setReadyToEndTurnStatus();
}

void View::setLostStatus(int wins, int lost) const
{
    m_viewMenu.setLostStatus(wins, lost);
}

void View::setWinStatus(int won, int lost) const
{
    m_viewMenu.setWinStatus(won, lost);
}

void View::drawScore(int won, int lost) const
{
    m_viewMenu.drawScore(won, lost);
}

void View::drawTimeLeft(std::chrono::milliseconds userTimeLeft, std::chrono::milliseconds enemyTimeLeft) const
{
    m_viewMenu.drawTimeLeft(userTimeLeft, enemyTimeLeft);
}

void View::drawUserTimeLeft(std::chrono::milliseconds timeLeft) const
{
    m_viewMenu.drawUserTimeLeft(timeLeft);
}

void View::drawEnemyTimeLeft(std::chrono::milliseconds timeLeft) const
{
    m_viewMenu.drawEnemyTimeLeft(timeLeft);
}

bool View::isStatusButton(unsigned int x, unsigned int y) const
{
    return m_viewMenu.isStatusButton(x, y);
}

std::optional<Direction> View::getMoveDirection(int x, int y) const
{
    const auto ballPos = m_board.getBallPosition();
    const auto viewX = ViewUtils::vx(ballPos.x);
    const auto viewY = ViewUtils::vy(ballPos.y);

    if (x == viewX - ViewUtils::X_FACTOR) {
        if (y == viewY - ViewUtils::Y_FACTOR) {
            return Direction::TopLeft;
        } else if (y == viewY) {
            return Direction::Left;
        } else if (y == viewY + ViewUtils::Y_FACTOR) {
            return Direction::BottomLeft;
        }
    } else if (x == viewX) {
        if (y == viewY - ViewUtils::Y_FACTOR) {
            return Direction::Top;
        } else if (y == viewY + ViewUtils::Y_FACTOR) {
            return Direction::Bottom;
        }
    } else if (x == viewX + ViewUtils::X_FACTOR) {
        if (y == viewY - ViewUtils::Y_FACTOR) {
            return Direction::TopRight;
        } else if (y == viewY) {
            return Direction::Right;
        } else if (y == viewY + ViewUtils::Y_FACTOR) {
            return Direction::BottomRight;
        }
    }

    return std::nullopt;
}

} // namespace PaperSoccer
