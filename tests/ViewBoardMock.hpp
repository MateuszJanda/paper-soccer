// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#ifndef VIEW_BOARD_MOCK_HPP
#define VIEW_BOARD_MOCK_HPP

#include "IViewBoard.hpp"
#include "gmock/gmock.h"

namespace PaperSoccer {

class ViewBoardMock : public IViewBoard {
public:
    MOCK_METHOD(void, drawBoard, (const std::string&, ColorPair, const std::string&, ColorPair,
        const std::vector<Direction>&, ColorPair), (const));
};

} // namespace PaperSoccer

#endif // VIEW_BOARD_MOCK_HPP
