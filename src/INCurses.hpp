#ifndef I_N_CURSES_HPP
#define I_N_CURSES_HPP

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
#endif // I_N_CURSES_HPP
