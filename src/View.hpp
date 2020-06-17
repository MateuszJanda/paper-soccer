#ifndef VIEW_HPP
#define VIEW_HPP

#include "Direction.hpp"
#include "IBoard.hpp"
#include "INCurses.hpp"
#include "IView.hpp"
#include <set>
#include <tuple>

namespace PaperSoccer {

using Skip = std::set<Direction>;
using Skips = std::tuple<Skip, Skip>;

class View : public IView {
public:
    View(IBoard& board, INCurses& ncurses);
    void drawBoard() override;
    void drawCell(Position nodePos, Skip nodeSkip, Skip neighSkip);

    Skips filterDirsForOutOfBorder(Position nodePos);
    Skips filterDirsForTopNetLine(Position nodePos);
    Skips filterDirsForBottomNetLine(Position nodePos);
    Skips filterDirsForTopBorderLine(Position nodePos);
    Skips filterDirsForRightLine(Position nodePos);

    void printText(int x, int y, std::string str);

    void setContinueStatus() override;
    void setEnemyTurnStatus() override;
    void setReadyToEndTurnStatus() override;
    void setLostStatus() override;
    void setWinStatus() override;

    bool isStatusButton(int x, int y) override;

    static constexpr int X_OFFSET{2};
    static constexpr int Y_OFFSET{2};
    static constexpr int X_FACTOR{3};
    static constexpr int Y_FACTOR{2};

private:
    void clearLines(Position nodePos);
    void drawVerticalToTopLine(Position nodePos);
    void drawHorizontalToRightLine(Position nodePos);
    void drawCrossToRight(Position nodePos);
    void drawHypotenuseToTopRight(Position nodePos);
    void drawHypotenuseToTopLeft(Position nodePos);
    void drawMarker(Position nodePos);

    void drawStatus(std::string line1, std::string line2);

    IBoard& m_board;
    INCurses& m_ncurses;
};

} // namespace PaperSoccer

#endif // VIEW_HPP
