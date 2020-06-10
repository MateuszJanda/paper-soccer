#ifndef VIEW_MOCK_HPP
#define VIEW_MOCK_HPP

#include "IView.hpp"
#include "gmock/gmock.h"

namespace PaperSoccer {

class ViewMock : public IView {
public:
    MOCK_METHOD0(drawBoard, void());
};

} // namespace PaperSoccer

#endif // VIEW_MOCK_HPP
