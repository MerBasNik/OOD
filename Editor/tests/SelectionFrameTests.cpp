//
// Created by Вадим Патрушев on 15.10.2026.
//

#include "Model/EditorState/SelectionFrame.h"
#include <gtest/gtest.h>

class SelectionFrameTest : public ::testing::Test
{
public:
	const Frame FRAME{ 100, 100, 200, 100 };
};

TEST_F(SelectionFrameTest, HitsCornerHandles)
{
	EXPECT_EQ(SelectionFrame::HitTestHandle(FRAME, { 100, 100 }), ResizeHandle::TopLeft);
	EXPECT_EQ(SelectionFrame::HitTestHandle(FRAME, { 300, 100 }), ResizeHandle::TopRight);
	EXPECT_EQ(SelectionFrame::HitTestHandle(FRAME, { 302, 202 }), ResizeHandle::BottomRight);
	EXPECT_EQ(SelectionFrame::HitTestHandle(FRAME, { 100, 200 }), ResizeHandle::BottomLeft);
}

TEST_F(SelectionFrameTest, HitsSideHandles)
{
	EXPECT_EQ(SelectionFrame::HitTestHandle(FRAME, { 200, 100 }), ResizeHandle::Top);
	EXPECT_EQ(SelectionFrame::HitTestHandle(FRAME, { 300, 150 }), ResizeHandle::Right);
	EXPECT_EQ(SelectionFrame::HitTestHandle(FRAME, { 200, 200 }), ResizeHandle::Bottom);
	EXPECT_EQ(SelectionFrame::HitTestHandle(FRAME, { 98, 150 }), ResizeHandle::Left);
}

TEST_F(SelectionFrameTest, MissesHandlesInsideFrame)
{
	EXPECT_EQ(SelectionFrame::HitTestHandle(FRAME, { 150, 150 }), std::nullopt);
}

TEST_F(SelectionFrameTest, HitsBorderNearFrameLines)
{
	EXPECT_TRUE(SelectionFrame::HitTestBorder(FRAME, { 150, 98 }));
	EXPECT_TRUE(SelectionFrame::HitTestBorder(FRAME, { 150, 103 }));
	EXPECT_TRUE(SelectionFrame::HitTestBorder(FRAME, { 302, 150 }));
}

TEST_F(SelectionFrameTest, MissesBorderInsideFrame)
{
	EXPECT_FALSE(SelectionFrame::HitTestBorder(FRAME, { 150, 150 }));
}

TEST_F(SelectionFrameTest, MissesBorderFarOutsideFrame)
{
	EXPECT_FALSE(SelectionFrame::HitTestBorder(FRAME, { 150, 90 }));
}
