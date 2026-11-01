//
// Created by Вадим Патрушев on 18.10.2026.
//

#include "Controller/FrameCalculator.h"
#include "Model/EditorState/Enums/ResizeHandle.h"
#include <gtest/gtest.h>

class FrameCalculatorTest : public ::testing::Test
{
public:
	const Frame BOUNDS{ 0, 0, 1000, 600 };
	const Frame START{ 100, 100, 200, 100 };
	const double MIN_SIZE = 10;

	Frame Resize(const ResizeHandle handle, const Point offset) const
	{
		return FrameCalculator::CalculateResizedFrame(START, handle, offset, BOUNDS, MIN_SIZE);
	}
};

TEST_F(FrameCalculatorTest, MoveShiftsFrameByOffset)
{
	EXPECT_EQ(FrameCalculator::CalculateMovedFrame(START, { 30, -20 }, BOUNDS), (Frame{ 130, 80, 200, 100 }));
}

TEST_F(FrameCalculatorTest, MoveIsLimitedByTopLeftOfBounds)
{
	EXPECT_EQ(FrameCalculator::CalculateMovedFrame(START, { -500, -500 }, BOUNDS), (Frame{ 0, 0, 200, 100 }));
}

TEST_F(FrameCalculatorTest, MoveIsLimitedByBottomRightOfBounds)
{
	EXPECT_EQ(FrameCalculator::CalculateMovedFrame(START, { 5000, 5000 }, BOUNDS), (Frame{ 800, 500, 200, 100 }));
}

TEST_F(FrameCalculatorTest, ResizeBySideHandleMovesOnlyThatSide)
{
	EXPECT_EQ(Resize(ResizeHandle::Right, { 10, 20 }), (Frame{ 100, 100, 210, 100 }));
}

TEST_F(FrameCalculatorTest, ResizeByTopHandleKeepsBottomInPlace)
{
	EXPECT_EQ(Resize(ResizeHandle::Top, { 10, -20 }), (Frame{ 100, 80, 200, 120 }));
}

TEST_F(FrameCalculatorTest, ResizeByCornerHandleMovesTwoSides)
{
	EXPECT_EQ(Resize(ResizeHandle::TopLeft, { -10, -20 }), (Frame{ 90, 80, 210, 120 }));
}

TEST_F(FrameCalculatorTest, ResizeByBottomRightIsLimitedByMinimumSize)
{
	EXPECT_EQ(Resize(ResizeHandle::BottomRight, { -500, -500 }), (Frame{ 100, 100, MIN_SIZE, MIN_SIZE }));
}

TEST_F(FrameCalculatorTest, ResizeByTopLeftIsLimitedByMinimumSize)
{
	EXPECT_EQ(Resize(ResizeHandle::TopLeft, { 500, 500 }), (Frame{ 290, 190, MIN_SIZE, MIN_SIZE }));
}

TEST_F(FrameCalculatorTest, ResizeIsLimitedByBounds)
{
	EXPECT_EQ(Resize(ResizeHandle::BottomRight, { 5000, 5000 }), (Frame{ 100, 100, 900, 500 }));
}

TEST_F(FrameCalculatorTest, CreateStretchesFrameFromStartToCursor)
{
	EXPECT_EQ(FrameCalculator::CalculateCreatedFrame({ 100, 100 }, { 300, 250 }, BOUNDS, MIN_SIZE),
		(Frame{ 100, 100, 200, 150 }));
}

TEST_F(FrameCalculatorTest, CreateTowardsTopLeftKeepsStartAsBottomRight)
{
	EXPECT_EQ(FrameCalculator::CalculateCreatedFrame({ 300, 250 }, { 100, 100 }, BOUNDS, MIN_SIZE),
		(Frame{ 100, 100, 200, 150 }));
}

TEST_F(FrameCalculatorTest, CreateIsLimitedByBounds)
{
	EXPECT_EQ(FrameCalculator::CalculateCreatedFrame({ 900, 500 }, { 5000, -5000 }, BOUNDS, MIN_SIZE),
		(Frame{ 900, 0, 100, 500 }));
}

TEST_F(FrameCalculatorTest, CreateIsLimitedByMinimumSize)
{
	EXPECT_EQ(FrameCalculator::CalculateCreatedFrame({ 100, 100 }, { 103, 98 }, BOUNDS, MIN_SIZE),
		(Frame{ 100, 90, MIN_SIZE, MIN_SIZE }));
}

TEST_F(FrameCalculatorTest, CreateNearBoundsGrowsAwayFromBorder)
{
	EXPECT_EQ(FrameCalculator::CalculateCreatedFrame({ 997, 2 }, { 999, 1 }, BOUNDS, MIN_SIZE),
		(Frame{ 990, 0, MIN_SIZE, MIN_SIZE }));
}

TEST_F(FrameCalculatorTest, CreateFromPointOutsideBoundsStartsAtBorder)
{
	EXPECT_EQ(FrameCalculator::CalculateCreatedFrame({ -50, 700 }, { 100, 500 }, BOUNDS, MIN_SIZE),
		(Frame{ 0, 500, 100, 100 }));
}

TEST_F(FrameCalculatorTest, GroupMoveKeepsRelativePositions)
{
	const auto frames = FrameCalculator::CalculateGroupMovedFrames({ START, { 400, 300, 50, 50 } }, { 30, -20 }, BOUNDS);

	EXPECT_EQ(frames, (std::vector<Frame>{ { 130, 80, 200, 100 }, { 430, 280, 50, 50 } }));
}

TEST_F(FrameCalculatorTest, GroupMoveIsLimitedByBoundsOfWholeGroup)
{
	const auto frames = FrameCalculator::CalculateGroupMovedFrames({ START, { 400, 300, 50, 50 } }, { 5000, 5000 }, BOUNDS);

	EXPECT_EQ(frames, (std::vector<Frame>{ { 650, 350, 200, 100 }, { 950, 550, 50, 50 } }));
}

TEST_F(FrameCalculatorTest, GroupResizeOfSingleFrameMatchesResize)
{
	const auto frames = FrameCalculator::CalculateGroupResizedFrames({ START }, 0, ResizeHandle::TopLeft, { -10, -20 },
		BOUNDS, MIN_SIZE);

	EXPECT_EQ(frames, std::vector{ Resize(ResizeHandle::TopLeft, { -10, -20 }) });
}

TEST_F(FrameCalculatorTest, GroupResizeScalesEachFrameRelativeToItsOppositeSide)
{
	const auto frames = FrameCalculator::CalculateGroupResizedFrames({ START, { 150, 300, 50, 50 } }, 0,
		ResizeHandle::Left, { -100, 0 }, BOUNDS, MIN_SIZE);

	EXPECT_EQ(frames, (std::vector<Frame>{ { 0, 100, 300, 100 }, { 125, 300, 75, 50 } }));
}

TEST_F(FrameCalculatorTest, GroupResizeIsLimitedByBoundsOfEachFrame)
{
	const auto frames = FrameCalculator::CalculateGroupResizedFrames({ START, { 0, 300, 50, 50 } }, 0,
		ResizeHandle::Left, { -100, 0 }, BOUNDS, MIN_SIZE);

	EXPECT_EQ(frames, (std::vector<Frame>{ START, { 0, 300, 50, 50 } }));
}
