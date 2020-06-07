#ifndef I_N_CURSES_HPP
#define I_N_CURSES_HPP

#include <string>
#include <tuple>

namespace PaperSoccer {

class INCurses {
public:
    virtual ~INCurses() = default;

    virtual void print(int x, int y, std::string str) = 0;
    virtual std::tuple<int, int, int> getChar() = 0;
    virtual void refreshView() = 0;
};

} // namespace PaperSoccer

#endif // I_N_CURSES_HPP
