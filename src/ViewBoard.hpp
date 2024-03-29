// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#ifndef VIEW_BOARD_HPP
#define VIEW_BOARD_HPP

#include "IBoard.hpp"
#include "INCurses.hpp"
#include "IViewBoard.hpp"
#include "ViewUtils.hpp"
#include "proto/Direction.pb.h"
#include <set>
#include <tuple>

namespace PaperSoccer {

enum MarkerVisibility {
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
    void drawCell(Position nodePos, Skip nodeSkip, Skip neighSkip, MarkerVisibility visibility) const;

    void drawNames(const std::string& topName, ColorPair topColor,
        const std::string& bottomName, ColorPair bottomColor) const;
    void drawPathMarkers(const std::vector<Direction>& dirPath, ColorPair ballColor) const;

    Skips filterDirsForOutOfBorder(Position nodePos) const;
    Skips filterDirsForTopNetLine(Position nodePos) const;
    Skips filterDirsForBottomNetLine(Position nodePos) const;
    Skips filterDirsForTopBorderLine(Position nodePos) const;
    Skips filterDirsForRightLine(Position nodePos) const;
    MarkerVisibility markerVisibility(Position nodePos) const;

private:
    void clearLines(const Position& nodePos) const;
    void drawVerticalToTopLine(const Position& nodePos) const;
    void drawHorizontalToRightLine(const Position& nodePos) const;
    void drawCrossToRight(const Position& nodePos) const;
    void drawHypotenuseToTopRight(const Position& nodePos) const;
    void drawHypotenuseToTopLeft(const Position& nodePos) const;
    void drawMarker(const Position& nodePos, MarkerVisibility visibility) const;

    const IBoard& m_board;
    const INCurses& m_ncurses;
};

} // namespace PaperSoccer

#endif // VIEW_BOARD_HPP
