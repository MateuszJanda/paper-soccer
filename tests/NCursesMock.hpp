#pragma once

#include "INCurses.hpp"
#include "gmock/gmock.h"

namespace PaperSoccer {

class NCursesMock : public INCurses {
public:
    MOCK_METHOD3(print, void(int, int, std::string));
    MOCK_METHOD0(getChar, std::tuple<int, int, int>());
};

} // namespace PaperSoccer
