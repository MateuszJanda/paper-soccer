// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#ifndef VIEW_MOCK_HPP
#define VIEW_MOCK_HPP

#include "IView.hpp"
#include "gmock/gmock.h"

namespace PaperSoccer {

class ViewMock : public IView {
public:
    using DirKeysMap = std::map<char, Direction>;

    MOCK_METHOD(void, clear, (), (const));
    MOCK_METHOD(void, drawBoard, (const std::string&, ColorPair, const std::string&, ColorPair,
        const std::vector<Direction>&, ColorPair), (const));
    MOCK_METHOD(void, drawLegend, (char, char, const DirKeysMap&), (const));

    MOCK_METHOD(void, setContinueStatus, (), (const));
    MOCK_METHOD(void, setEnemyTurnStatus, (), (const));
    MOCK_METHOD(void, setReadyToEndTurnStatus, (), (const));
    MOCK_METHOD(void, setLostStatus, (int, int), (const));
    MOCK_METHOD(void, setWinStatus, (int, int), (const));
    MOCK_METHOD(void, drawScore, (int, int), (const));

    MOCK_METHOD(void, drawTimeLeft, (std::chrono::milliseconds, std::chrono::milliseconds), (const));
    MOCK_METHOD(void, drawUserTimeLeft, (std::chrono::milliseconds), (const));
    MOCK_METHOD(void, drawEnemyTimeLeft, (std::chrono::milliseconds), (const));

    MOCK_METHOD(bool, isStatusButton, (unsigned int, unsigned int), (const));
    MOCK_METHOD(std::optional<Direction>, getMoveDirection, (int, int), (const));
};

} // namespace PaperSoccer

#endif // VIEW_MOCK_HPP
