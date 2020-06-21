#ifndef VIEW_MOCK_HPP
#define VIEW_MOCK_HPP

#include "IView.hpp"
#include "gmock/gmock.h"

namespace PaperSoccer {

class ViewMock : public IView {
public:
    MOCK_METHOD3(drawBoard, void(std::string, std::string, std::vector<Direction>));
    MOCK_METHOD3(drawLegend, void(char, char, std::map<char, Direction>));
    MOCK_METHOD2(drawScore, void(int, int));
    MOCK_METHOD0(setContinueStatus, void());
    MOCK_METHOD0(setEnemyTurnStatus, void());
    MOCK_METHOD0(setReadyToEndTurnStatus, void());
    MOCK_METHOD2(setLostStatus, void(int, int));
    MOCK_METHOD2(setWinStatus, void(int, int));
    MOCK_CONST_METHOD2(isStatusButton, bool(int, int));
    MOCK_CONST_METHOD2(getMoveDirection, std::optional<Direction>(int, int));

    MOCK_METHOD3(printText, void(int, int, std::string));
};

} // namespace PaperSoccer

#endif // VIEW_MOCK_HPP
