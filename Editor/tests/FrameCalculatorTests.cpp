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
