#ifndef NCURSES_MOCK_HPP
#define NCURSES_MOCK_HPP

#include "INCurses.hpp"
#include "gmock/gmock.h"

namespace PaperSoccer {

class NCursesMock : public INCurses {
public:
    MOCK_METHOD3(print, void(int, int, std::string));
    MOCK_METHOD0(getInput, std::optional<Input>());
    MOCK_METHOD0(refreshView, void());
    MOCK_METHOD0(clearView, void());
};

} // namespace PaperSoccer

#endif // NCURSES_MOCK_HPP
