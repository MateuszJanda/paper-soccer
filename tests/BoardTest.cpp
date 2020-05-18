#include "gtest/gtest.h"
#include "Board.hpp"

using namespace Ps;

namespace
{
constexpr std::size_t HEIGHT{10};
constexpr std::size_t WIDTH{8};
}

class BoardTest : public testing::Test
{
public:
    Board board{WIDTH, HEIGHT};
};


TEST_F(BoardTest, first)
{
    ASSERT_EQ(board.getWidth(), WIDTH);
    ASSERT_EQ(board.getHeight(), HEIGHT);
}
