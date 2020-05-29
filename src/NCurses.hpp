#pragma once

#include "INCurses.hpp"

namespace PaperSoccer {

class NCurses : public INCurses {
public:
    NCurses();
    void print(int x, int y, std::string str) override;
    std::tuple<int, int, int> getChar() override;

    ~NCurses();
};

}
