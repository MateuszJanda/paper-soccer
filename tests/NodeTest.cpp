#include "Node.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace PaperSoccer {

using namespace testing;

class NodeTest : public testing::Test {
public:
    Node node;

    std::array<Direction, 8> allDirs{
        Direction::Top,
        Direction::TopLeft,
        Direction::Right,
        Direction::BottomRight,
        Direction::Bottom,
        Direction::BottomLeft,
        Direction::Left,
        Direction::TopRight};
};

TEST_F(NodeTest, checkAddNeighbour)
{
    for (const auto& dir : allDirs) {
        ASSERT_FALSE(node.hasNeighbour(dir));
        ASSERT_TRUE(node.addNeighbour(dir));
        ASSERT_TRUE(node.hasNeighbour(dir));
    }
}

TEST_F(NodeTest, checkAddTwoNeighbours)
{
    ASSERT_FALSE(node.hasNeighbour(Direction::Top));
    ASSERT_FALSE(node.hasNeighbour(Direction::Left));

    node.addNeighbours({Direction::Top, Direction::Left});

    ASSERT_TRUE(node.hasNeighbour(Direction::Top));
    ASSERT_TRUE(node.hasNeighbour(Direction::Left));
}

TEST_F(NodeTest, checkAddTwiceSameNieghbour)
{
    ASSERT_FALSE(node.hasNeighbour(Direction::Top));

    ASSERT_TRUE(node.addNeighbour(Direction::Top));
    ASSERT_FALSE(node.addNeighbour(Direction::Top));

    ASSERT_TRUE(node.hasNeighbour(Direction::Top));
}

TEST_F(NodeTest, checkDeletingNotExistingNeighbour)
{
    ASSERT_FALSE(node.hasNeighbour(Direction::Top));
    ASSERT_FALSE(node.delNeighbour(Direction::Top));
    ASSERT_FALSE(node.hasNeighbour(Direction::Top));
}

TEST_F(NodeTest, checkDeletingExistingNeighbour)
{
    ASSERT_TRUE(node.addNeighbour(Direction::Top));
    ASSERT_TRUE(node.hasNeighbour(Direction::Top));

    ASSERT_TRUE(node.delNeighbour(Direction::Top));
    ASSERT_FALSE(node.hasNeighbour(Direction::Top));
}

TEST_F(NodeTest, checkDegree)
{
    ASSERT_EQ(node.degree(), 0);

    for (int counter = 0; counter < allDirs.size(); counter++) {
        ASSERT_TRUE(node.addNeighbour(allDirs[counter]));
        ASSERT_EQ(node.degree(), counter + 1);
    }
}

TEST_F(NodeTest, checkDegreeDoesNotChangeForSameNeighbour)
{
    ASSERT_EQ(node.degree(), 0);

    ASSERT_TRUE(node.addNeighbour(Direction::Top));
    ASSERT_FALSE(node.addNeighbour(Direction::Top));

    ASSERT_EQ(node.degree(), 1);
}

TEST_F(NodeTest, whenTwoNeighbourAdded_thenReturnDegreeTwo)
{
    ASSERT_TRUE(node.addNeighbour(Direction::Top));
    ASSERT_TRUE(node.addNeighbour(Direction::Left));

    ASSERT_EQ(node.degree(), 2);
}

TEST_F(NodeTest, checkCanEnter)
{
    std::array<Direction, 6> dirs{
        Direction::Top,
        Direction::TopRight,
        Direction::Right,
        Direction::BottomRight,
        Direction::Bottom,
        Direction::BottomLeft,
    };

    ASSERT_TRUE(node.canEnter());
    for (const auto& dir : dirs) {
        ASSERT_TRUE(node.addNeighbour(dir));
        ASSERT_TRUE(node.canEnter());
    }

    ASSERT_TRUE(node.addNeighbour(Direction::Left));
    ASSERT_FALSE(node.canEnter());
    ASSERT_TRUE(node.addNeighbour(Direction::TopLeft));
    ASSERT_FALSE(node.canEnter());
}

TEST_F(NodeTest, checkIsLonely)
{
    ASSERT_TRUE(node.isLonely());
    ASSERT_TRUE(node.addNeighbour(Direction::Left));
    ASSERT_FALSE(node.isLonely());
}

TEST_F(NodeTest, checkNeighboursPositionsIfNoNeighbours)
{
    ASSERT_THAT(node.neighboursPositions(Position{1, 1}), ElementsAre());
}

TEST_F(NodeTest, checkNeighboursPositions)
{
    ASSERT_TRUE(node.addNeighbour(Direction::Top));
    ASSERT_TRUE(node.addNeighbour(Direction::Left));

    ASSERT_THAT(node.neighboursPositions(Position{1, 1}), ElementsAre(Position{1, 0}, Position{0, 1}));
}

}
