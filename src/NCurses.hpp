// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#ifndef N_CURSES_HPP
#define N_CURSES_HPP

#include "INCurses.hpp"

namespace PaperSoccer {

class NCurses : public INCurses {
public:
    NCurses();
    ~NCurses() override;

    void print(int x, int y, std::string str) const override;
    std::optional<Input> getInput() const noexcept override;

    void refreshView() const noexcept override;
    void clearView() const noexcept override;

private:
    void initColors() const;
    void prepareColor(int colorId, int red, int green, int blue) const;
};

} // namespace PaperSoccer

#endif // N_CURSES_HPP
