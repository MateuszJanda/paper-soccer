// paper-soccer - a free terminal version of paper soccer game
// Copyright (C) 2020 Mateusz Janda <mateusz janda at gmail com>
// Homepage: github.com/MateuszJanda/paper-soccer
// Ad maiorem Dei gloriam

#include "Position.hpp"
#include "gtest/gtest.h"

namespace PaperSoccer {

using namespace testing;

TEST(PositionTest, checkEqual)
{
    Position p1{1, 1};
    Position p2{1, 1};

    ASSERT_EQ(p1, p2);
}

TEST(PositionTest, checkNotEqual)
{
    Position p1{1, 1};
    Position p2{1, 2};

    ASSERT_FALSE(p1 == p2);
}

TEST(PositionTest, checkAdd)
{
    Position p1{1, 1};
    p1 += std::make_pair<int>(1, 2);

    Position p2{2, 3};
    ASSERT_EQ(p1, p2);
}

} // namespace PaperSoccer
