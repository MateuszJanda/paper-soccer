#include "MoveMsg.hpp"

namespace PaperSoccer {

MoveMsg::MoveMsg(Direction dir)
    : header{Header::TmpMove}
    , dir{dir}
{
}

} // namespace PaperSoccer
