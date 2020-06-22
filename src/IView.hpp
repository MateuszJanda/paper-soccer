#ifndef I_VIEW_HPP
#define I_VIEW_HPP

#include "Direction.hpp"
#include "ColorPair.hpp"
#include <string>
#include <vector>
#include <map>
#include <optional>

namespace PaperSoccer {

class IView {
public:
    virtual ~IView() = default;

    virtual void clear() const = 0;
    virtual void drawBoard(std::string topName, ColorPair topColor,
                           std::string bottomName, ColorPair bottomColor,
                           std::vector<Direction> dirPath, ColorPair ballColor) const = 0;
    virtual void drawLegend(char undo, char newGame, std::map<char, Direction> dirKeys) const = 0;
    virtual void drawScore(int won, int lost) const = 0;
    virtual void setContinueStatus() const = 0;
    virtual void setEnemyTurnStatus() const = 0;
    virtual void setReadyToEndTurnStatus() const = 0;
    virtual void setLostStatus(int won, int lost) const = 0;
    virtual void setWinStatus(int won, int lost) const = 0;
    virtual bool isStatusButton(int x, int y) const = 0;
    virtual std::optional<Direction> getMoveDirection(int x, int y) const = 0;
};

} // namespace PaperSoccer

#endif // I_VIEW_HPP
