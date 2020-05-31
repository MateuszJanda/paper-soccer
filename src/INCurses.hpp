#ifndef INCURSES_HPP
#define INCURSES_HPP

#include <string>
#include <tuple>

namespace PaperSoccer {

class INCurses {
public:
    virtual void print(int x, int y, std::string str) = 0;
    virtual std::tuple<int, int, int> getChar() = 0;

    virtual ~INCurses() = default;
};

}
#endif // INCURSES_HPP
