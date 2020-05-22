#pragma once

#include "Direction.hpp"
#include "NCurses.hpp"
#include <set>


namespace PaperSoccer {

class Board;

class View
{
public:
    View();
    void drawBoard(const Board &board);
    void drawDebugBoard(const Board& board);
    void drawCell(const Board& board, PaperSoccer::Position nodePos, std::set<PaperSoccer::Direction> skip);

private:
    void drawVerticalToTopLine(Position nodePos);
    void drawHorizontalToRightLine(Position nodePos);
    void drawCrossToRight(Position nodePos);
    void drawHypotenuseToTopRight(Position nodePos);
    void drawMarker(Position nodePos);

    NCurses nnn;
};

}
