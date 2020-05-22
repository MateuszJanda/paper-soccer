#include "View.hpp"
#include "BoardMock.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace PaperSoccer
{

using namespace testing;

class ViewTest : public testing::Test {
public:
    ViewTest() : view(boardMock)
    {

    }
    StrictMock<BoardMock> boardMock;
    View view;
};

TEST_F(ViewTest, checkAddNeighbour)
{
}

}
