#pragma once

#include "Direction.hpp"
#include "NCurses.hpp"
#include "IBoard.hpp"
#include <set>


namespace PaperSoccer {

class Board;

class View
{
public:
    View(IBoard &board);
    void drawBoard();
    void drawCell(PaperSoccer::Position nodePos, std::set<PaperSoccer::Direction> skip);

private:
    std::set<Direction> filterDirsForOutOfBorder(Position nodePos);
    std::set<Direction> filterDirsForTopBorderLine(Position nodePos);
    std::set<Direction> filterDirsForBottomBorderLine(Position nodePos);

    void drawVerticalToTopLine(Position nodePos);
    void drawHorizontalToRightLine(Position nodePos);
    void drawCrossToRight(Position nodePos);
    void drawHypotenuseToTopRight(Position nodePos);
    void drawMarker(Position nodePos);

    NCurses nnn;
    IBoard& m_board;


};

}
