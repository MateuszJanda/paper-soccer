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
    void drawBoard(std::string topName, std::string bottomName, std::vector<Direction> dirPath) const override;
    void drawCell(Position nodePos, Skip nodeSkip, Skip neighSkip, MarkerVisability visability) const;

    void drawNames(std::string topName, std::string bottomName) const;
    void drawPathMarkers(std::vector<Direction> dirPath) const;

    Skips filterDirsForOutOfBorder(Position nodePos) const;
    Skips filterDirsForTopNetLine(Position nodePos) const;
    Skips filterDirsForBottomNetLine(Position nodePos) const;
    Skips filterDirsForTopBorderLine(Position nodePos) const;
    Skips filterDirsForRightLine(Position nodePos) const;
    MarkerVisability markerVisability(Position nodePos) const;

    void drawLegend(char undo, char newGame, std::map<char, Direction> dirKeys) const override;

    void printText(int x, int y, std::string str) const override;

    void drawScore(int won, int lost) const override;
    void setContinueStatus() const override;
    void setEnemyTurnStatus() const override;
    void setReadyToEndTurnStatus() const override;
    void setLostStatus(int won, int lost) const override;
    void setWinStatus(int won, int lost) const override;

    bool isStatusButton(int x, int y) const override;
    std::optional<Direction> getMoveDirection(int x, int y) const override;

    static constexpr int X_OFFSET{2};
    static constexpr int Y_OFFSET{2};
    static constexpr int X_FACTOR{3};
    static constexpr int Y_FACTOR{2};
    const std::string LINE{"+--------------+"};

private:
    void clearLines(Position nodePos) const;
    void drawVerticalToTopLine(Position nodePos) const;
    void drawHorizontalToRightLine(Position nodePos) const;
    void drawCrossToRight(Position nodePos) const;
    void drawHypotenuseToTopRight(Position nodePos) const;
    void drawHypotenuseToTopLeft(Position nodePos) const;
    void drawMarker(Position nodePos, MarkerVisability visability) const;

    void drawStatusButton(std::string line1, std::string line2) const;
    int getStatusButtonXShift() const;

    const IBoard& m_board;
    const INCurses& m_ncurses;
};

} // namespace PaperSoccer

#endif // VIEW_HPP
