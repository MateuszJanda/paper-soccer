#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Node.hpp"

using namespace Ps;
using namespace testing;

class NodeTest : public testing::Test {
public:
    Node node;

    std::array<Direction, 8> allDirs {
        Direction::top,
        Direction::top_left,
        Direction::right,
        Direction::bottom_right,
        Direction::bottom,
        Direction::bottom_left,
        Direction::left,
        Direction::top_right
    };
};

TEST_F(NodeTest, checkAddNeighbour) {
    for (const auto& dir : allDirs) {
        ASSERT_FALSE(node.hasNeighbour(dir));
        ASSERT_TRUE(node.addNeighbour(dir));
        ASSERT_TRUE(node.hasNeighbour(dir));
    }
}

TEST_F(NodeTest, checkAddTwiceSameNieghbour) {
    ASSERT_FALSE(node.hasNeighbour(Direction::top));

    ASSERT_TRUE(node.addNeighbour(Direction::top));
    ASSERT_FALSE(node.addNeighbour(Direction::top));

    ASSERT_TRUE(node.hasNeighbour(Direction::top));
}

TEST_F(NodeTest, checkDeletingNotExistingNeighbour) {
    ASSERT_FALSE(node.hasNeighbour(Direction::top));
    ASSERT_FALSE(node.delNeighbour(Direction::top));
    ASSERT_FALSE(node.hasNeighbour(Direction::top));
}

TEST_F(NodeTest, checkDeletingExistingNeighbour) {
    ASSERT_TRUE(node.addNeighbour(Direction::top));
    ASSERT_TRUE(node.hasNeighbour(Direction::top));

    ASSERT_TRUE(node.delNeighbour(Direction::top));
    ASSERT_FALSE(node.hasNeighbour(Direction::top));
}

TEST_F(NodeTest, checkDegree) {
    ASSERT_EQ(node.degree(), 0);

    for (int counter = 0; counter < allDirs.size(); counter++) {
        ASSERT_TRUE(node.addNeighbour(allDirs[counter]));
        ASSERT_EQ(node.degree(), counter + 1);
    }
}

TEST_F(NodeTest, checkDegreeDoesNotChangeForSameNeighbour) {
    ASSERT_EQ(node.degree(), 0);

    ASSERT_TRUE(node.addNeighbour(Direction::top));
    ASSERT_FALSE(node.addNeighbour(Direction::top));

    ASSERT_EQ(node.degree(), 1);
}

TEST_F(NodeTest, whenTwoNeighbourAdded_thenReturnDegreeTwo) {
    ASSERT_TRUE(node.addNeighbour(Direction::top));
    ASSERT_TRUE(node.addNeighbour(Direction::left));

    ASSERT_EQ(node.degree(), 2);
}

TEST_F(NodeTest, checkCanEnter) {
    std::array<Direction, 6> dirs {
        Direction::top,
        Direction::top_right,
        Direction::right,
        Direction::bottom_right,
        Direction::bottom,
        Direction::bottom_left,
    };

    ASSERT_TRUE(node.canEnter());
    for (const auto& dir : dirs) {
        ASSERT_TRUE(node.addNeighbour(dir));
        ASSERT_TRUE(node.canEnter());
    }

    ASSERT_TRUE(node.addNeighbour(Direction::left));
    ASSERT_FALSE(node.canEnter());
    ASSERT_TRUE(node.addNeighbour(Direction::top_left));
    ASSERT_FALSE(node.canEnter());
}

TEST_F(NodeTest, checkIsLonely) {
    ASSERT_TRUE(node.isLonely());
    ASSERT_TRUE(node.addNeighbour(Direction::left));
    ASSERT_FALSE(node.isLonely());
}

TEST_F(NodeTest, checkNeighboursPositionsIfNoNeighbours) {
    ASSERT_THAT(node.neighboursPositions(Position{1, 1}), ElementsAre());
}

TEST_F(NodeTest, checkNeighboursPositions) {
    ASSERT_TRUE(node.addNeighbour(Direction::top));
    ASSERT_TRUE(node.addNeighbour(Direction::left));

    ASSERT_THAT(node.neighboursPositions(Position{1, 1}), ElementsAre(Position{1, 0}, Position{0, 1}));
}

