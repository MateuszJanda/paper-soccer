#pragma once


namespace Ps {

class Position {
public:
    Position(int x, int y)
        : x(x), y(y)
    {
    }

    void operator +=(std::pair<int, int> offset) {
        x += offset.first;
        y += offset.second;
    }

    int x;
    int y;
};

} // namespace Ps
