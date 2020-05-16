#include "gtest/gtest.h"
#include "Board.hpp"

namespace
{
constexpr std::size_t WIDTH{8};
constexpr std::size_t HEIGHT{10};
}

class BoardTest : public testing::Test
{
public:
    Board<HEIGHT, WIDTH> board;
};


TEST_F(BoardTest, first)
{
//    ASSERT_EQ(board.getWidth(), WIDTH);
    ASSERT_EQ(board.getHeight(), HEIGHT);
}
