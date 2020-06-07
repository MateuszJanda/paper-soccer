#ifndef N_CURSES_HPP
#define N_CURSES_HPP

#include "INCurses.hpp"

namespace PaperSoccer {

void rawPrint(int x, int y, std::string str);

class NCurses : public INCurses {
public:
    NCurses();
    void print(int x, int y, std::string str) override;
    std::tuple<int, int, int> getChar() override;

    void refreshView() override;

    ~NCurses();
};

} // namespace PaperSoccer

#endif // N_CURSES_HPP
