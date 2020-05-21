#pragma once

#include "Direction.hpp"
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
};

}
