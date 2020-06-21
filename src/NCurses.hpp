#ifndef N_CURSES_HPP
#define N_CURSES_HPP

#include "INCurses.hpp"

namespace PaperSoccer {

void rawPrint(int x, int y, std::string str);

class NCurses : public INCurses {
public:
    NCurses();
    ~NCurses() override;

    void print(int x, int y, std::string str) const override;
    std::optional<Input> getInput() const override;

    void refreshView() const override;
    void clearView() const override;
};

} // namespace PaperSoccer

#endif // N_CURSES_HPP
