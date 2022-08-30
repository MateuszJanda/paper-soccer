// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#ifndef VIEW_MENU_HPP
#define VIEW_MENU_HPP

#include "IBoard.hpp"
#include "INCurses.hpp"
#include "IViewMenu.hpp"
#include "ViewUtils.hpp"
#include "proto/Direction.pb.h"

namespace PaperSoccer {

class ViewMenu : public IViewMenu {
public:
    ViewMenu(const IBoard& board, const INCurses& ncurses);

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
    void drawTime(unsigned int x, unsigned int y, std::chrono::milliseconds timeLeft, const std::string& name, ColorPair color) const;

    bool isStatusButton(unsigned int x, unsigned int y) const override;

private:
    const std::string TOP_LINE{".--------------."};
    const std::string BOTTOM_LINE{"`--------------'"};
    const std::size_t BUTTON_HEIGHT{3};

    void drawStatusButton(const std::string& line1, const std::string& line2, ColorPair color) const;
    unsigned int getMenuXOffset() const;

    const IBoard& m_board;
    const INCurses& m_ncurses;
};

} // namespace PaperSoccer

#endif // VIEW_MENU_HPP
