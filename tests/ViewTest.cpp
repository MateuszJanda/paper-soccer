#include "View.hpp"
#include "BoardMock.hpp"
#include "NCursesMock.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace PaperSoccer {

namespace {
    const Skip EMPTY;
    const Skip ALL{Direction::Top, Direction::TopRight, Direction::Right};
    const Skip TOP_TOPRIGHT{Direction::Top, Direction::TopRight};
    const Skip TOPRIGHT{Direction::TopRight};
    const Skip TOPLEFT{Direction::TopLeft};
    const Skip TOPRIGHT_RIGHT{Direction::TopRight, Direction::Right};
    const Skip TOP_RIGHT{Direction::Top, Direction::Right};
    const Skip RIGHT{Direction::Right};

    constexpr std::size_t WIDTH{9};
    constexpr std::size_t HEIGHT{13};

    constexpr std::size_t TOP_NET_LINE{0};
    constexpr std::size_t BOTTOM_NET_LINE{HEIGHT - 1};
    constexpr std::size_t TOP_BORDER_LINE{TOP_NET_LINE + 1};
    constexpr std::size_t BOTTOM_BORDER_LINE{BOTTOM_NET_LINE - 1};
    constexpr std::size_t CENTER_LINE{6};
    constexpr std::size_t RIGHT_LINE{WIDTH - 1};
    constexpr std::size_t GOALPOST_LEFT{3};
    constexpr std::size_t GOALPOST_RIGHT{5};

    const Position NODE_POS{0, 0};
    const Position NEIGHBOUR_POS{NODE_POS.x + 1, NODE_POS.y};
}

using namespace testing;

class ViewTest : public testing::Test {
public:
    ViewTest()
        : view(boardMock, ncursesMock)
    {
        EXPECT_CALL(boardMock, getGoalpostLeft()).WillRepeatedly(Return(GOALPOST_LEFT));
        EXPECT_CALL(boardMock, getGoalpostRight()).WillRepeatedly(Return(GOALPOST_RIGHT));
        EXPECT_CALL(boardMock, getHeight()).WillRepeatedly(Return(HEIGHT));
        EXPECT_CALL(boardMock, getWidth()).WillRepeatedly(Return(WIDTH));
    }

    void expectClearLines(Position nodePos = NODE_POS)
    {
        int x = nodePos.x * View::X_FACTOR + View::X_OFFSET;
        int y = nodePos.y * View::Y_FACTOR - 1 + View::Y_OFFSET;
        EXPECT_CALL(ncursesMock, print(x, y, " "));

        x = nodePos.x * View::X_FACTOR + 1 + View::X_OFFSET;
        y = nodePos.y * View::Y_FACTOR + View::Y_OFFSET;
        EXPECT_CALL(ncursesMock, print(x, y, "  "));

        x = nodePos.x * View::X_FACTOR + 1 + View::X_OFFSET;
        y = nodePos.y * View::Y_FACTOR - 1 + View::Y_OFFSET;
        EXPECT_CALL(ncursesMock, print(x, y, "  "));
    }

    void expectDrawStatus()
    {
        EXPECT_CALL(ncursesMock, print(_, View::Y_OFFSET + 0, _));
        EXPECT_CALL(ncursesMock, print(_, View::Y_OFFSET + 1, _));
        EXPECT_CALL(ncursesMock, print(_, View::Y_OFFSET + 2, _));
        EXPECT_CALL(ncursesMock, print(_, View::Y_OFFSET + 3, _));
        EXPECT_CALL(ncursesMock, refreshView());
    }

    StrictMock<BoardMock> boardMock;
    StrictMock<NCursesMock> ncursesMock;
    View view;
};

TEST_F(ViewTest, checkFilterDirsForTopNetLineWhenNotThisLine)
{
    const auto [nodeSkip, neighSkip] = view.filterDirsForTopNetLine(Position{0, TOP_BORDER_LINE});
    ASSERT_EQ(nodeSkip, EMPTY);
    ASSERT_EQ(neighSkip, EMPTY);
}

TEST_F(ViewTest, checkFilterDirsForTopNetLineWhenCorner)
{
    const auto [nodeSkip, neighSkip] = view.filterDirsForTopNetLine(Position{0, TOP_NET_LINE});
    ASSERT_EQ(nodeSkip, ALL);
    ASSERT_EQ(neighSkip, TOPLEFT);
}

TEST_F(ViewTest, checkFilterDirsForTopNetLineWhenGoalpostLeft)
{
    const auto [nodeSkip, neighSkip] = view.filterDirsForTopNetLine(Position{GOALPOST_LEFT, TOP_NET_LINE});
    ASSERT_EQ(nodeSkip, TOP_TOPRIGHT);
    ASSERT_EQ(neighSkip, TOPLEFT);
}

TEST_F(ViewTest, checkFilterDirsForBottomBorderLineWhenNotThisLine)
{
    const auto [nodeSkip, neighSkip] = view.filterDirsForBottomNetLine(Position{0, TOP_BORDER_LINE});
    ASSERT_EQ(nodeSkip, EMPTY);
    ASSERT_EQ(neighSkip, EMPTY);
}

TEST_F(ViewTest, checkFilterDirsForBottomBorderLineWhenNotAGoal)
{
    const auto [nodeSkip, neighSkip] = view.filterDirsForBottomNetLine(Position{0, BOTTOM_NET_LINE});
    ASSERT_EQ(nodeSkip, ALL);
    ASSERT_EQ(neighSkip, TOPLEFT);
}

TEST_F(ViewTest, checkFilterDirsForBottomBorderLineWhenInTheMiddleOfGoal)
{
    const auto [nodeSkip, neighSkip] = view.filterDirsForBottomNetLine(Position{GOALPOST_LEFT + 1, BOTTOM_NET_LINE});
    ASSERT_EQ(nodeSkip, EMPTY);
    ASSERT_EQ(neighSkip, EMPTY);
}

TEST_F(ViewTest, checkFilterDirsForBottomBorderLineWhenGoalpostRight)
{
    const auto [nodeSkip, neighSkip] = view.filterDirsForBottomNetLine(Position{GOALPOST_RIGHT, BOTTOM_NET_LINE});
    ASSERT_EQ(nodeSkip, TOPRIGHT_RIGHT);
    ASSERT_EQ(neighSkip, TOPLEFT);
}

TEST_F(ViewTest, checkFilterDirsForTopBorderLineWhenNotThisLine)
{
    const auto [nodeSkip, neighSkip] = view.filterDirsForTopBorderLine(Position{0, TOP_NET_LINE});
    ASSERT_EQ(nodeSkip, EMPTY);
    ASSERT_EQ(neighSkip, EMPTY);
}

TEST_F(ViewTest, checkFilterDirsForTopBorderLineWhenCorner)
{
    const auto [nodeSkip, neighSkip] = view.filterDirsForTopBorderLine(Position{0, TOP_BORDER_LINE});
    ASSERT_EQ(nodeSkip, TOP_TOPRIGHT);
    ASSERT_EQ(neighSkip, TOPLEFT);
}

TEST_F(ViewTest, checkFilterDirsForTopBorderLineWhenGoalpostLeft)
{
    const auto [nodeSkip, neighSkip] = view.filterDirsForTopBorderLine(Position{GOALPOST_LEFT, TOP_BORDER_LINE});
    ASSERT_EQ(nodeSkip, EMPTY);
    ASSERT_EQ(neighSkip, EMPTY);
}

TEST_F(ViewTest, checkFilterDirsForTopBorderLineWhenGoalpostRight)
{
    const auto [nodeSkip, neighSkip] = view.filterDirsForTopBorderLine(Position{GOALPOST_RIGHT, TOP_BORDER_LINE});
    ASSERT_EQ(nodeSkip, TOPRIGHT);
    ASSERT_EQ(neighSkip, TOPLEFT);
}

TEST_F(ViewTest, checkFilterDirsForRightLineWhenNotThisLine)
{
    const auto [nodeSkip, neighSkip] = view.filterDirsForRightLine(Position{0, CENTER_LINE});
    ASSERT_EQ(nodeSkip, EMPTY);
    ASSERT_EQ(neighSkip, EMPTY);
}

TEST_F(ViewTest, checkFilterDirsForRightLineWhenThisLine)
{
    const auto [nodeSkip, neighSkip] = view.filterDirsForRightLine(Position{RIGHT_LINE, CENTER_LINE});
    ASSERT_EQ(nodeSkip, TOPRIGHT_RIGHT);
    ASSERT_EQ(neighSkip, EMPTY);
}

TEST_F(ViewTest, checkDrawCellPlusMarkerSkipAllDirs)
{
    expectClearLines();

    int x = NODE_POS.x * View::X_FACTOR + View::X_OFFSET;
    int y = NODE_POS.y * View::Y_FACTOR + View::Y_OFFSET;
    EXPECT_CALL(ncursesMock, print(x, y, "+"));

    view.drawCell(NODE_POS, ALL, TOPLEFT);
}

TEST_F(ViewTest, checkDrawCellPlusMarkerNoNeighbour)
{
    expectClearLines();

    EXPECT_CALL(boardMock, hasNeighbour(NODE_POS, Direction::Top)).WillOnce(Return(false));

    int x = NODE_POS.x * View::X_FACTOR + View::X_OFFSET;
    int y = NODE_POS.y * View::Y_FACTOR + View::Y_OFFSET;
    EXPECT_CALL(ncursesMock, print(x, y, "+"));

    view.drawCell(NODE_POS, TOPRIGHT_RIGHT, TOPLEFT);
}

TEST_F(ViewTest, checkDrawCellTopPath)
{
    expectClearLines();

    EXPECT_CALL(boardMock, hasNeighbour(NODE_POS, Direction::Top)).WillOnce(Return(true));

    int x = NODE_POS.x * View::X_FACTOR + View::X_OFFSET;
    int y = NODE_POS.y * View::Y_FACTOR - 1 + View::Y_OFFSET;
    EXPECT_CALL(ncursesMock, print(x, y, "|"));

    x = NODE_POS.x * View::X_FACTOR + View::X_OFFSET;
    y = NODE_POS.y * View::Y_FACTOR + View::Y_OFFSET;
    EXPECT_CALL(ncursesMock, print(x, y, "+"));

    view.drawCell(NODE_POS, TOPRIGHT_RIGHT, TOPLEFT);
}

TEST_F(ViewTest, checkDrawCellRightPathNoNeighbour)
{
    expectClearLines();

    EXPECT_CALL(boardMock, hasNeighbour(NODE_POS, Direction::Right)).WillOnce(Return(false));

    int x = NODE_POS.x * View::X_FACTOR + View::X_OFFSET;
    int y = NODE_POS.y * View::Y_FACTOR + View::Y_OFFSET;
    EXPECT_CALL(ncursesMock, print(x, y, "+"));

    view.drawCell(NODE_POS, TOP_TOPRIGHT, TOPLEFT);
}

TEST_F(ViewTest, checkDrawCellRightPath)
{
    expectClearLines();

    EXPECT_CALL(boardMock, hasNeighbour(NODE_POS, Direction::Right)).WillOnce(Return(true));

    int x = NODE_POS.x * View::X_FACTOR + 1 + View::X_OFFSET;
    int y = NODE_POS.y * View::Y_FACTOR + View::Y_OFFSET;
    EXPECT_CALL(ncursesMock, print(x, y, "--"));

    x = NODE_POS.x * View::X_FACTOR + View::X_OFFSET;
    y = NODE_POS.y * View::Y_FACTOR + View::Y_OFFSET;
    EXPECT_CALL(ncursesMock, print(x, y, "+"));

    view.drawCell(NODE_POS, TOP_TOPRIGHT, TOPLEFT);
}

TEST_F(ViewTest, checkDrawCellTopRightPathNoNeighbour)
{
    expectClearLines();

    EXPECT_CALL(boardMock, hasNeighbour(NODE_POS, Direction::TopRight)).WillOnce(Return(false));

    int x = NODE_POS.x * View::X_FACTOR + View::X_OFFSET;
    int y = NODE_POS.y * View::Y_FACTOR + View::Y_OFFSET;
    EXPECT_CALL(ncursesMock, print(x, y, "+"));

    view.drawCell(NODE_POS, TOP_RIGHT, TOPLEFT);
}

TEST_F(ViewTest, checkDrawCellTopRightPath)
{
    expectClearLines();

    EXPECT_CALL(boardMock, hasNeighbour(NODE_POS, Direction::TopRight)).WillOnce(Return(true));

    int x = NODE_POS.x * View::X_FACTOR + 1 + View::X_OFFSET;
    int y = NODE_POS.y * View::Y_FACTOR - 1 + View::Y_OFFSET;
    EXPECT_CALL(ncursesMock, print(x, y, "⸝⸍"));

    x = NODE_POS.x * View::X_FACTOR + View::X_OFFSET;
    y = NODE_POS.y * View::Y_FACTOR + View::Y_OFFSET;
    EXPECT_CALL(ncursesMock, print(x, y, "+"));

    view.drawCell(NODE_POS, TOP_RIGHT, TOPLEFT);
}

TEST_F(ViewTest, checkDrawCellTopLeftPathNeighbourOutOfRange)
{
    Position nodePos{WIDTH - 1, 0};
    expectClearLines(nodePos);

    int x = nodePos.x * View::X_FACTOR + View::X_OFFSET;
    int y = nodePos.y * View::Y_FACTOR + View::Y_OFFSET;
    EXPECT_CALL(ncursesMock, print(x, y, "+"));

    view.drawCell(nodePos, ALL, EMPTY);
}

TEST_F(ViewTest, checkDrawCellTopLeftPathNoNeighbour)
{
    expectClearLines();

    EXPECT_CALL(boardMock, hasNeighbour(NEIGHBOUR_POS, Direction::TopLeft)).WillOnce(Return(false));

    int x = NODE_POS.x * View::X_FACTOR + View::X_OFFSET;
    int y = NODE_POS.y * View::Y_FACTOR + View::Y_OFFSET;
    EXPECT_CALL(ncursesMock, print(x, y, "+"));

    view.drawCell(NODE_POS, ALL, EMPTY);
}

TEST_F(ViewTest, checkDrawCellTopLeftPath)
{
    expectClearLines();

    EXPECT_CALL(boardMock, hasNeighbour(NEIGHBOUR_POS, Direction::TopLeft)).WillOnce(Return(true));

    int x = NEIGHBOUR_POS.x * View::X_FACTOR - 2 + View::X_OFFSET;
    int y = NEIGHBOUR_POS.y * View::Y_FACTOR - 1 + View::Y_OFFSET;
    EXPECT_CALL(ncursesMock, print(x, y, "⸌⸜"));

    x = NODE_POS.x * View::X_FACTOR + View::X_OFFSET;
    y = NODE_POS.x * View::Y_FACTOR + View::Y_OFFSET;
    EXPECT_CALL(ncursesMock, print(x, y, "+"));

    view.drawCell(NODE_POS, ALL, EMPTY);
}

TEST_F(ViewTest, checkDrawCellCrossPath)
{
    expectClearLines();

    EXPECT_CALL(boardMock, hasNeighbour(NODE_POS, Direction::TopRight)).WillOnce(Return(true));
    EXPECT_CALL(boardMock, hasNeighbour(NEIGHBOUR_POS, Direction::TopLeft)).WillOnce(Return(true));

    int x = NODE_POS.x * View::X_FACTOR + 1 + View::X_OFFSET;
    int y = NODE_POS.y * View::Y_FACTOR - 1 + View::Y_OFFSET;
    EXPECT_CALL(ncursesMock, print(x, y, "ᐳᐸ"));

    x = NODE_POS.x * View::X_FACTOR + View::X_OFFSET;
    y = NODE_POS.x * View::Y_FACTOR + View::Y_OFFSET;
    EXPECT_CALL(ncursesMock, print(x, y, "+"));

    view.drawCell(NODE_POS, TOP_RIGHT, EMPTY);
}

TEST_F(ViewTest, checkSetContinueStatus)
{
    expectDrawStatus();
    view.setContinueStatus();
}

TEST_F(ViewTest, checkSetEnemyTurnStatus)
{
    expectDrawStatus();
    view.setEnemyTurnStatus();
}

TEST_F(ViewTest, checkSetReadyToEndTurnStatus)
{
    expectDrawStatus();
    view.setReadyToEndTurnStatus();
}

TEST_F(ViewTest, checkSetLostStatus)
{
    expectDrawStatus();
    view.setLostStatus();
}

TEST_F(ViewTest, checkSetWinStatus)
{
    expectDrawStatus();
    view.setWinStatus();
}

TEST_F(ViewTest, checkIsStatusButtonNotClicked)
{
    ASSERT_FALSE(view.isStatusButton(0, 0));
}

TEST_F(ViewTest, checkIsStatusButtonClicked)
{
    const auto x = View::X_OFFSET + WIDTH * View::X_FACTOR + 2;
    const auto y = View::Y_OFFSET;
    ASSERT_TRUE(view.isStatusButton(x, y));
}

TEST_F(ViewTest, checkGetMouseDirection)
{
    const Position ballPos{1, 2};
    EXPECT_CALL(boardMock, getBallPosition()).WillOnce(Return(ballPos));

    ASSERT_EQ(view.getMouseDirection(1, 2), std::nullopt);
}

} // namespace PaperSoccer
