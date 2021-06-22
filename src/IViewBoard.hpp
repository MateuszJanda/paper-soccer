#ifndef I_VIEW_BOARD_HPP
#define I_VIEW_BOARD_HPP

#include "ColorPair.hpp"
#include "DirectionUtils.hpp"
#include <chrono>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace PaperSoccer {

class IViewBoard {
public:
    virtual ~IViewBoard() noexcept = default;

    virtual void drawBoard(const std::string& topName, ColorPair topColor,
        const std::string& bottomName, ColorPair bottomColor,
        const std::vector<Direction>& dirPath, ColorPair ballColor) const = 0;
};

} // namespace PaperSoccer

#endif // I_VIEW_BOARD_HPP
