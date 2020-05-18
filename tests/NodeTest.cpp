#include "gtest/gtest.h"
#include "Node.hpp"

using namespace Ps;


class NodeTest : public testing::Test {
public:
    Node node;
};


TEST_F(NodeTest, whenFirstNeighbourAdded_addReturnTrue) {
    ASSERT_FALSE(node.hasNeighbour(Direction::top));

    ASSERT_TRUE(node.addNeighbour(Direction::top));

    ASSERT_TRUE(node.hasNeighbour(Direction::top));
}

TEST_F(NodeTest, whenSameNeighbourAdded_addReturnFalse) {
    ASSERT_FALSE(node.hasNeighbour(Direction::top));

    ASSERT_TRUE(node.addNeighbour(Direction::top));
    ASSERT_FALSE(node.addNeighbour(Direction::top));

    ASSERT_TRUE(node.hasNeighbour(Direction::top));
}

TEST_F(NodeTest, whenNoNeighbourAdded_thenReturnDegreeZero) {
    ASSERT_EQ(node.degree(), 0);
}

TEST_F(NodeTest, whenTwoNeighbourAdded_thenReturnDegreeTwo) {
    ASSERT_TRUE(node.addNeighbour(Direction::top));
    ASSERT_TRUE(node.addNeighbour(Direction::left));

    ASSERT_EQ(node.degree(), 2);
}
