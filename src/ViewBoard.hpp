// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#ifndef VIEW_BOARD_HPP
#define VIEW_BOARD_HPP

#include "DirectionUtils.hpp"
#include "ViewUtils.hpp"
#include "IBoard.hpp"
#include "INCurses.hpp"
#include "IViewBoard.hpp"
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

class ViewBoard : public IViewBoard {
public:
    ViewBoard(const IBoard& board, const INCurses& ncurses);
    void drawBoard(const std::string& topName, ColorPair topColor,
        const std::string& bottomName, ColorPair bottomColor,
        const std::vector<Direction>& dirPath, ColorPair ballColor) const override;
    void drawCell(Position nodePos, Skip nodeSkip, Skip neighSkip, MarkerVisability visability) const;

    void drawNames(const std::string& topName, ColorPair topColor,
        const std::string& bottomName, ColorPair bottomColor) const;
    void drawPathMarkers(const std::vector<Direction>& dirPath, ColorPair ballColor) const;

    Skips filterDirsForOutOfBorder(Position nodePos) const;
    Skips filterDirsForTopNetLine(Position nodePos) const;
    Skips filterDirsForBottomNetLine(Position nodePos) const;
    Skips filterDirsForTopBorderLine(Position nodePos) const;
    Skips filterDirsForRightLine(Position nodePos) const;
    MarkerVisability markerVisability(Position nodePos) const;

private:
    const std::string TOP_LINE{".--------------."};
    const std::string BOTTOM_LINE{"`--------------'"};
    const std::size_t BUTTON_HEIGHT{3};

    void clearLines(const Position& nodePos) const;
    void drawVerticalToTopLine(const Position& nodePos) const;
    void drawHorizontalToRightLine(const Position& nodePos) const;
    void drawCrossToRight(const Position& nodePos) const;
    void drawHypotenuseToTopRight(const Position& nodePos) const;
    void drawHypotenuseToTopLeft(const Position& nodePos) const;
    void drawMarker(const Position& nodePos, MarkerVisability visability) const;

    const IBoard& m_board;
    const INCurses& m_ncurses;
};

} // namespace PaperSoccer

#endif // VIEW_BOARD_HPP
