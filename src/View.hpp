#ifndef VIEW_HPP
#define VIEW_HPP

#include "Direction.hpp"
#include "IBoard.hpp"
#include "INCurses.hpp"
#include <set>
#include <tuple>

namespace PaperSoccer {

using Skip = std::set<Direction>;
using Skips = std::tuple<Skip, Skip>;

class View {
public:
    View(IBoard& board, INCurses& ncurses);
    void drawBoard();
    void drawCell(Position nodePos, Skip nodeSkip, Skip neighSkip);

    Skips filterDirsForOutOfBorder(Position nodePos);
    Skips filterDirsForTopNetLine(Position nodePos);
    Skips filterDirsForBottomNetLine(Position nodePos);
    Skips filterDirsForTopBorderLine(Position nodePos);
    Skips filterDirsForRightLine(Position nodePos);

    void printText(int x, int y, std::string str);

    static constexpr int X_OFFSET{2};
    static constexpr int Y_OFFSET{2};

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

} // namespace PaperSoccer

#endif // VIEW_HPP
