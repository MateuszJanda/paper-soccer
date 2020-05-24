#pragma once

#include <string>

namespace PaperSoccer {

class INCurses {
public:
    virtual void print(int x, int y, std::string str) = 0;

    virtual ~INCurses() = default;
};

}
