#ifndef I_VIEW_HPP
#define I_VIEW_HPP

#include <string>

namespace PaperSoccer {

class IView {
public:
    virtual ~IView() = default;

    virtual void drawBoard() = 0;
    virtual void setContinueStatus() = 0;
    virtual void setEnemyTurnStatus() = 0;
    virtual void setReadyToEndTurnStatus() = 0;
    virtual void setLostStatus() = 0;
    virtual void setWinStatus() = 0;
    virtual bool isStatusButton(int x, int y) = 0;

    virtual void printText(int x, int y, std::string s) = 0;
};

} // namespace PaperSoccer

#endif // I_VIEW_HPP
