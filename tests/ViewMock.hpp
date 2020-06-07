#pragma once

#include "IView.hpp"
#include "gmock/gmock.h"

namespace PaperSoccer {

class ViewMock : public IView {
public:
    MOCK_METHOD0(drawBoard, void());
};

} // namespace PaperSoccer
