#ifndef I_VIEW_MENU_HPP
#define I_VIEW_MENU_HPP

#include "proto/Direction.pb.h"
#include <chrono>
#include <map>

namespace PaperSoccer {

class IViewMenu {
public:
    virtual ~IViewMenu() noexcept = default;

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

    virtual bool isStatusButton(unsigned int x, unsigned int y) const = 0;
};

} // namespace PaperSoccer

#endif // I_VIEW_MENU_HPP
