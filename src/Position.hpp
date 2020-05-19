#pragma once


namespace PaperSoccer {

class Position {
public:
    constexpr Position(int x, int y)
        : x(x), y(y)
    {
    }

    constexpr Position& operator +=(const std::pair<int, int>& offset) {
        x += offset.first;
        y += offset.second;
        return *this;
    }

    constexpr bool operator ==(const Position& second) const {
        return x == second.x and y == second.y;
    }

    int x;
    int y;
};

} // namespace PaperSoccer
