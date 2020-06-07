#ifndef I_VIEW_HPP
#define I_VIEW_HPP

namespace PaperSoccer {

class IView {
public:
    virtual ~IView() = default;

    virtual void drawBoard() = 0;
};

} // namespace PaperSoccer

#endif // I_VIEW_HPP
