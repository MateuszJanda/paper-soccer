#pragma once

#include "Direction.hpp"
#include "INCurses.hpp"
#include "IBoard.hpp"
#include <set>
#include <tuple>

namespace PaperSoccer {

using Skip = std::set<Direction>;
using Skips = std::tuple<Skip, Skip>;

class View
{

public:
    View(IBoard &board, INCurses& ncurses);
    void drawBoard();
    void drawCell(Position nodePos, Skip nodeSkip, Skip neighSkip);

    Skips filterDirsForOutOfBorder(Position nodePos);
    Skips filterDirsForTopNetLine(Position nodePos);
    Skips filterDirsForBottomNetLine(Position nodePos);
    Skips filterDirsForTopBorderLine(Position nodePos);
    Skips filterDirsForRightLine(Position nodePos);

private:
    void drawVerticalToTopLine(Position nodePos);
    void drawHorizontalToRightLine(Position nodePos);
    void drawCrossToRight(Position nodePos);
    void drawHypotenuseToTopRight(Position nodePos);
    void drawHypotenuseToTopLeft(Position nodePos);
    void drawMarker(Position nodePos);

    IBoard& m_board;
    INCurses& m_ncurses;
};

}
