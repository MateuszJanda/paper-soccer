#include "TmpMoveMsg.hpp"

namespace PaperSoccer {

TmpMoveMsg::TmpMoveMsg(Direction dir)
    : header{Header::TmpMove}
    , dir{dir}
{
}

}
