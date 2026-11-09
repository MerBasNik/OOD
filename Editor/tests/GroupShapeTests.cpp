//
// Created by Вадим Патрушев on 09.11.2026.
//

#include "Mock/MockCanvas.h"
#include "Mock/ShapeTypeVisitor.h"
#include "Model/Rendering/RenderShape.h"
#include "Model/Shapes/Ellipse.h"
#include "Model/Shapes/GroupShape.h"
#include "Model/Shapes/Rectangle.h"
#include "Model/Shapes/Triangle.h"
#include <gtest/gtest.h>

using ::testing::InSequence;
using ::testing::StrictMock;

class GroupShapeTest : public ::testing::Test
{
public:
	static std::unique_ptr<IShape> MakeRectangle(const Frame& frame, const Color color = 0)
	{
		return std::make_unique<Rectangle>(frame, color, 0);
	}

	static std::unique_ptr<GroupShape> MakeGroup(std::unique_ptr<IShape> first, std::unique_ptr<IShape> second)
	{
		std::vector<std::unique_ptr<IShape>> children;
		children.push_back(std::move(first));
		children.push_back(std::move(second));
		return std::make_unique<GroupShape>(std::move(children));
	}

	static std::unique_ptr<GroupShape> MakeNestedGroup()
	{
		auto inner = MakeGroup(std::make_unique<Triangle>(Frame{ 100, 100, 100, 100 }, 0, 0),
			MakeRectangle({ 200, 200, 100, 100 }));
		return MakeGroup(MakeRectangle({ 0, 0, 100, 100 }), std::move(inner));
	}
};

TEST_F(GroupShapeTest, FrameCoversChildren)
{
	const auto group = MakeGroup(MakeRectangle({ 10, 20, 100, 50 }), MakeRectangle({ 200, 0, 50, 300 }));

	EXPECT_EQ(group->GetFrame(), (Frame{ 10, 0, 240, 300 }));
}

TEST_F(GroupShapeTest, MoveShiftsChildren)
{
	const auto group = MakeGroup(MakeRectangle({ 0, 0, 100, 100 }), std::make_unique<Ellipse>(Frame{ 200, 0, 100, 100 }, 0, 0));

	group->SetFrame({ 50, 30, 300, 100 });

	EXPECT_EQ(group->GetChild(0).GetFrame(), (Frame{ 50, 30, 100, 100 }));
	EXPECT_EQ(group->GetChild(1).GetFrame(), (Frame{ 250, 30, 100, 100 }));
}

TEST_F(GroupShapeTest, ResizeScalesChildren)
{
	const auto group = MakeGroup(MakeRectangle({ 0, 0, 100, 100 }), std::make_unique<Ellipse>(Frame{ 200, 0, 100, 100 }, 0, 0));

	group->SetFrame({ 0, 0, 600, 200 });

	EXPECT_EQ(group->GetChild(0).GetFrame(), (Frame{ 0, 0, 200, 200 }));
	EXPECT_EQ(group->GetChild(1).GetFrame(), (Frame{ 400, 0, 200, 200 }));
	EXPECT_EQ(group->GetFrame(), (Frame{ 0, 0, 600, 200 }));
}

TEST_F(GroupShapeTest, ResizeScalesNestedGroups)
{
	const auto group = MakeNestedGroup();
	ASSERT_EQ(group->GetFrame(), (Frame{ 0, 0, 300, 300 }));

	group->SetFrame({ 10, 10, 150, 150 });

	const auto* inner = group->GetChild(1).GetGroup();
	ASSERT_NE(inner, nullptr);
	EXPECT_EQ(group->GetChild(0).GetFrame(), (Frame{ 10, 10, 50, 50 }));
	EXPECT_EQ(inner->GetChild(0).GetFrame(), (Frame{ 60, 60, 50, 50 }));
	EXPECT_EQ(inner->GetChild(1).GetFrame(), (Frame{ 110, 110, 50, 50 }));
}

TEST_F(GroupShapeTest, ResizeToZeroThrows)
{
	const auto group = MakeGroup(MakeRectangle({ 0, 0, 100, 100 }), MakeRectangle({ 200, 0, 100, 100 }));

	EXPECT_THROW(group->SetFrame({ 0, 0, 0, 100 }), std::invalid_argument);
	EXPECT_EQ(group->GetFrame(), (Frame{ 0, 0, 300, 100 }));
}

TEST_F(GroupShapeTest, HitTestIgnoresEmptySpace)
{
	const auto group = MakeGroup(MakeRectangle({ 0, 0, 100, 100 }), MakeRectangle({ 200, 0, 100, 100 }));

	EXPECT_TRUE(group->HitTest({ 50, 50 }));
	EXPECT_TRUE(group->HitTest({ 250, 50 }));
	EXPECT_FALSE(group->HitTest({ 150, 50 }));
}

TEST_F(GroupShapeTest, CloneIsDeepCopy)
{
	const auto group = MakeNestedGroup();

	const auto clone = group->Clone();
	clone->SetFrame({ 0, 0, 600, 600 });

	EXPECT_EQ(group->GetFrame(), (Frame{ 0, 0, 300, 300 }));
	EXPECT_EQ(clone->GetFrame(), (Frame{ 0, 0, 600, 600 }));
	EXPECT_EQ(GetShapeType(*clone), ShapeType::Group);
}

TEST_F(GroupShapeTest, TimeIntervalCoversChildren)
{
	auto first = MakeRectangle({ 0, 0, 100, 100 });
	first->SetTimeInterval({ 2, 4 });
	auto second = MakeRectangle({ 200, 0, 100, 100 });
	second->SetTimeInterval({ 5, 8 });
	const auto group = MakeGroup(std::move(first), std::move(second));

	EXPECT_EQ(group->GetTimeInterval(), (TimeInterval{ 2, 8 }));

	group->SetTimeInterval({ 3, 9 });

	EXPECT_EQ(group->GetChild(0).GetTimeInterval(), (TimeInterval{ 3, 5 }));
	EXPECT_EQ(group->GetChild(1).GetTimeInterval(), (TimeInterval{ 6, 9 }));
}

TEST_F(GroupShapeTest, RenderDrawsAllInZOrder)
{
	StrictMock<MockCanvas> canvas;
	RenderShape renderer(canvas);
	const auto group = MakeNestedGroup();
	{
		InSequence sequence;
		EXPECT_CALL(canvas, DrawRectangle(Frame{ 0, 0, 100, 100 }, 0, 0));
		EXPECT_CALL(canvas, DrawPolygon(::testing::_, 0, 0));
		EXPECT_CALL(canvas, DrawRectangle(Frame{ 200, 200, 100, 100 }, 0, 0));
	}

	group->Accept(renderer);
}

TEST_F(GroupShapeTest, RenderSkipsHiddenChildren)
{
	StrictMock<MockCanvas> canvas;
	RenderShape renderer(canvas, 1);
	auto hidden = MakeRectangle({ 200, 0, 100, 100 });
	hidden->SetTimeInterval({ 5, 8 });
	const auto group = MakeGroup(MakeRectangle({ 0, 0, 100, 100 }), std::move(hidden));
	EXPECT_CALL(canvas, DrawRectangle(Frame{ 0, 0, 100, 100 }, 0, 0));

	group->Accept(renderer);
}
