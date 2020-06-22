// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#ifndef VIEW_HPP
#define VIEW_HPP

#include "Direction.hpp"
#include "IBoard.hpp"
#include "INCurses.hpp"
#include "IView.hpp"
#include <set>
#include <tuple>

namespace PaperSoccer {

enum MarkerVisability {
    Invisible,
    NotOccupied,
    Occupied
};

using Skip = std::set<Direction>;
using Skips = std::tuple<Skip, Skip>;

class View : public IView {
public:
    View(const IBoard& board, const INCurses& ncurses);
    void clear() const override;
    void drawBoard(std::string topName, ColorPair topColor,
                   std::string bottomName, ColorPair bottomColor,
                   std::vector<Direction> dirPath, ColorPair ballColor) const override;
    void drawCell(Position nodePos, Skip nodeSkip, Skip neighSkip, MarkerVisability visability) const;

    void drawNames(std::string topName, ColorPair topColor, std::string bottomName, ColorPair bottomColor) const;
    void drawPathMarkers(std::vector<Direction> dirPath, ColorPair ballColor) const;

    Skips filterDirsForOutOfBorder(Position nodePos) const;
    Skips filterDirsForTopNetLine(Position nodePos) const;
    Skips filterDirsForBottomNetLine(Position nodePos) const;
    Skips filterDirsForTopBorderLine(Position nodePos) const;
    Skips filterDirsForRightLine(Position nodePos) const;
    MarkerVisability markerVisability(Position nodePos) const;

    void drawLegend(char undo, char newGame, std::map<char, Direction> dirKeys) const override;

    void drawScore(int won, int lost) const override;
    void setContinueStatus() const override;
    void setEnemyTurnStatus() const override;
    void setReadyToEndTurnStatus() const override;
    void setLostStatus(int won, int lost) const override;
    void setWinStatus(int won, int lost) const override;

    bool isStatusButton(int x, int y) const override;
    std::optional<Direction> getMoveDirection(int x, int y) const override;

    static constexpr int X_FACTOR{3};
    static constexpr int Y_FACTOR{2};

    static constexpr int X_OFFSET{2};
    static constexpr int Y_OFFSET{1};
    static constexpr int Y_SCORE_OFFSET{Y_OFFSET + 5};
    static constexpr int Y_LEGNENT_OFFSET{Y_OFFSET + 9};

private:
    const std::string TOP_LINE{".--------------."};
    const std::string BOTTOM_LINE{"`--------------'"};
    const std::size_t BUTTON_HEIGHT{3};

    void clearLines(Position nodePos) const;
    void drawVerticalToTopLine(Position nodePos) const;
    void drawHorizontalToRightLine(Position nodePos) const;
    void drawCrossToRight(Position nodePos) const;
    void drawHypotenuseToTopRight(Position nodePos) const;
    void drawHypotenuseToTopLeft(Position nodePos) const;
    void drawMarker(Position nodePos, MarkerVisability visability) const;

    void drawStatusButton(std::string line1, std::string line2, ColorPair color) const;
    int getMenuXOffset() const;

    const IBoard& m_board;
    const INCurses& m_ncurses;
};

} // namespace PaperSoccer

#endif // VIEW_HPP
