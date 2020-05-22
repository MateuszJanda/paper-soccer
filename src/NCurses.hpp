#pragma once

#include "INCurses.hpp"
#include <string>

namespace PaperSoccer {


class NCurses : public INCurses {
public:
    NCurses();
    void print(int y, int x, std::string str);

    ~NCurses();
};

}
