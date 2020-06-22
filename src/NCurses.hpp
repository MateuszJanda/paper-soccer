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

    void print(int x, int y, std::string str, ColorPair colorPair = ColorPair::DEFAULT) const override;
    std::optional<Input> getInput() const noexcept override;

    void refreshView() const noexcept override;
    void clearView() const noexcept override;

    static constexpr int DEFAULT_FOREGROUND{1};
    static constexpr int DEFAULT_BACKGROUND{2};
    static constexpr int RED{3};
    static constexpr int BLUE{4};
    static constexpr int GRAY{5};
    static constexpr int YELLOW{6};
    static constexpr int GREEN{7};

private:
    void setupColors() const;
    void prepareColor(int colorId, int red, int green, int blue) const;
    void prepareColorPair(ColorPair colorPair, int fg, int bg) const;
};

} // namespace PaperSoccer

#endif // N_CURSES_HPP
