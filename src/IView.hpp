#ifndef I_VIEW_HPP
#define I_VIEW_HPP

#include "ColorPair.hpp"
#include "DirectionUtils.hpp"
#include <chrono>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace PaperSoccer {

class IView {
public:
    virtual ~IView() noexcept = default;

    virtual void clear() const = 0;
    virtual void drawBoard(const std::string& topName, ColorPair topColor,
        const std::string& bottomName, ColorPair bottomColor,
        const std::vector<Direction>& dirPath, ColorPair ballColor) const = 0;
    virtual void drawLegend(char undo, char newGame, const std::map<char, Direction>& dirKeys) const = 0;

    virtual void setContinueStatus() const = 0;
    virtual void setEnemyTurnStatus() const = 0;
    virtual void setReadyToEndTurnStatus() const = 0;
    virtual void setLostStatus(int won, int lost) const = 0;
    virtual void setWinStatus(int won, int lost) const = 0;
    virtual void drawScore(int won, int lost) const = 0;

    virtual void drawTimeLeft(std::chrono::milliseconds userTimeLeft, std::chrono::milliseconds enemyTimeLeft) const = 0;
    virtual void drawUserTimeLeft(std::chrono::milliseconds timeLeft) const = 0;
    virtual void drawEnemyTimeLeft(std::chrono::milliseconds timeLeft) const = 0;

    virtual bool isStatusButton(int x, int y) const = 0;
    virtual std::optional<Direction> getMoveDirection(int x, int y) const = 0;
};

} // namespace PaperSoccer

#endif // I_VIEW_HPP
