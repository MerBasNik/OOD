//
// Created by Вадим Патрушев on 15.10.2026.
//

#include "Model/EditorState/SelectionGeometry.h"
#include <gtest/gtest.h>

class SelectionGeometryTest : public ::testing::Test
{
public:
	const Frame FRAME{ 100, 100, 200, 100 };
};

TEST_F(SelectionGeometryTest, HitsCornerHandles)
{
	EXPECT_EQ(SelectionGeometry::HitTestHandle(FRAME, { 100, 100 }), ResizeHandle::TopLeft);
	EXPECT_EQ(SelectionGeometry::HitTestHandle(FRAME, { 300, 100 }), ResizeHandle::TopRight);
	EXPECT_EQ(SelectionGeometry::HitTestHandle(FRAME, { 302, 202 }), ResizeHandle::BottomRight);
	EXPECT_EQ(SelectionGeometry::HitTestHandle(FRAME, { 100, 200 }), ResizeHandle::BottomLeft);
}

TEST_F(SelectionGeometryTest, HitsSideHandles)
{
	EXPECT_EQ(SelectionGeometry::HitTestHandle(FRAME, { 200, 100 }), ResizeHandle::Top);
	EXPECT_EQ(SelectionGeometry::HitTestHandle(FRAME, { 300, 150 }), ResizeHandle::Right);
	EXPECT_EQ(SelectionGeometry::HitTestHandle(FRAME, { 200, 200 }), ResizeHandle::Bottom);
	EXPECT_EQ(SelectionGeometry::HitTestHandle(FRAME, { 98, 150 }), ResizeHandle::Left);
}

TEST_F(SelectionGeometryTest, MissesHandlesInsideFrame)
{
	EXPECT_EQ(SelectionGeometry::HitTestHandle(FRAME, { 150, 150 }), std::nullopt);
}

TEST_F(SelectionGeometryTest, HitsBorderNearFrameLines)
{
	EXPECT_TRUE(SelectionGeometry::HitTestBorder(FRAME, { 150, 98 }));
	EXPECT_TRUE(SelectionGeometry::HitTestBorder(FRAME, { 150, 103 }));
	EXPECT_TRUE(SelectionGeometry::HitTestBorder(FRAME, { 302, 150 }));
}

TEST_F(SelectionGeometryTest, MissesBorderInsideFrame)
{
	EXPECT_FALSE(SelectionGeometry::HitTestBorder(FRAME, { 150, 150 }));
}

TEST_F(SelectionGeometryTest, MissesBorderFarOutsideFrame)
{
	EXPECT_FALSE(SelectionGeometry::HitTestBorder(FRAME, { 150, 90 }));
}
