#ifndef VIEW_MOCK_HPP
#define VIEW_MOCK_HPP

#include "IView.hpp"
#include "gmock/gmock.h"

namespace PaperSoccer {

class ViewMock : public IView {
public:
    MOCK_METHOD0(drawBoard, void());
    MOCK_METHOD0(setContinueStatus, void());
    MOCK_METHOD0(setEnemyTurnStatus, void());
    MOCK_METHOD0(setReadyToEndTurnStatus, void());
    MOCK_METHOD0(setLostStatus, void());
    MOCK_METHOD0(setWinStatus, void());
    MOCK_CONST_METHOD2(isStatusButton, bool(int, int));
    MOCK_CONST_METHOD2(getMouseDirection, std::optional<Direction>(int, int));

    MOCK_METHOD3(printText, void(int, int, std::string));
};

} // namespace PaperSoccer

#endif // VIEW_MOCK_HPP
