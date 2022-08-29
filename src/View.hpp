// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#ifndef VIEW_HPP
#define VIEW_HPP

#include "DirectionUtils.hpp"
#include "IBoard.hpp"
#include "INCurses.hpp"
#include "IView.hpp"
#include "IViewBoard.hpp"
#include "IViewMenu.hpp"
#include "ViewUtils.hpp"
#include <set>
#include <tuple>

namespace PaperSoccer {

class View : public IView {
public:
    View(const IBoard& board, const INCurses& ncurses, const IViewBoard& viewBoard, const IViewMenu& viewMenu);
    void clear() const override;
    void drawBoard(const std::string& topName, ColorPair topColor,
        const std::string& bottomName, ColorPair bottomColor,
        const std::vector<Direction>& dirPath, ColorPair ballColor) const override;

    void drawLegend(char undo, char newGame, const std::map<char, Direction>& dirKeys) const override;

    void setContinueStatus() const override;
    void setEnemyTurnStatus() const override;
    void setReadyToEndTurnStatus() const override;
    void setLostStatus(int won, int lost) const override;
    void setWinStatus(int won, int lost) const override;
    void drawScore(int won, int lost) const override;

    void drawTimeLeft(std::chrono::milliseconds userTimeLeft, std::chrono::milliseconds enemyTimeLeft) const override;
    void drawUserTimeLeft(std::chrono::milliseconds timeLeft) const override;
    void drawEnemyTimeLeft(std::chrono::milliseconds timeLeft) const override;

    bool isStatusButton(unsigned int x, unsigned int y) const override;
    std::optional<Direction> getMoveDirection(int x, int y) const override;

private:
    const IBoard& m_board;
    const INCurses& m_ncurses;
    const IViewBoard& m_viewBoard;
    const IViewMenu& m_viewMenu;
};

} // namespace PaperSoccer

#endif // VIEW_HPP
