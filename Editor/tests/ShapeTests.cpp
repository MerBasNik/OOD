//
// Created by Вадим Патрушев on 15.10.2026.
//

#include "Model/Shapes/Ellipse.h"
#include "Model/Shapes/Rectangle.h"
#include "Model/Shapes/Triangle.h"
#include <gtest/gtest.h>

class ShapeTest : public ::testing::Test
{
public:
	const Frame FRAME{ 100, 100, 200, 100 };
	const Color COLOR = 0x123456;
};

TEST_F(ShapeTest, RectangleHitsPointsInsideFrame)
{
	const Rectangle shape(FRAME, COLOR);

	EXPECT_TRUE(shape.HitTest({ 100, 100 }));
	EXPECT_TRUE(shape.HitTest({ 299, 199 }));
}

TEST_F(ShapeTest, RectangleMissesPointsOutsideFrame)
{
	const Rectangle shape(FRAME, COLOR);

	EXPECT_FALSE(shape.HitTest({ 99, 150 }));
	EXPECT_FALSE(shape.HitTest({ 200, 201 }));
}

TEST_F(ShapeTest, EllipseHitsPointsInsideEllipse)
{
	const Ellipse shape(FRAME, COLOR);

	EXPECT_TRUE(shape.HitTest({ 200, 150 }));
	EXPECT_TRUE(shape.HitTest({ 101, 150 }));
	EXPECT_TRUE(shape.HitTest({ 200, 101 }));
}

TEST_F(ShapeTest, EllipseMissesBoundingBoxCorners)
{
	const Ellipse shape(FRAME, COLOR);

	EXPECT_FALSE(shape.HitTest({ 105, 105 }));
	EXPECT_FALSE(shape.HitTest({ 295, 195 }));
}

TEST_F(ShapeTest, TriangleHitsPointsInsideTriangle)
{
	const Triangle shape(FRAME, COLOR);

	EXPECT_TRUE(shape.HitTest({ 200, 101 }));
	EXPECT_TRUE(shape.HitTest({ 200, 150 }));
	EXPECT_TRUE(shape.HitTest({ 105, 199 }));
}

TEST_F(ShapeTest, TriangleMissesBoundingBoxCorners)
{
	const Triangle shape(FRAME, COLOR);

	EXPECT_FALSE(shape.HitTest({ 105, 105 }));
	EXPECT_FALSE(shape.HitTest({ 295, 105 }));
}

TEST_F(ShapeTest, TriangleMissesPointsOutsideFrame)
{
	const Triangle shape(FRAME, COLOR);

	EXPECT_FALSE(shape.HitTest({ 200, 201 }));
}

TEST_F(ShapeTest, ShapesReportTheirType)
{
	EXPECT_EQ(Rectangle(FRAME, COLOR).GetType(), ShapeType::Rectangle);
	EXPECT_EQ(Ellipse(FRAME, COLOR).GetType(), ShapeType::Ellipse);
	EXPECT_EQ(Triangle(FRAME, COLOR).GetType(), ShapeType::Triangle);
}

TEST_F(ShapeTest, ShapeKeepsFrameAndColorFromConstructor)
{
	const Ellipse shape(FRAME, COLOR);

	EXPECT_EQ(shape.GetFrame(), FRAME);
	EXPECT_EQ(shape.GetColor(), COLOR);
}

TEST_F(ShapeTest, SetFrameChangesFrame)
{
	Rectangle shape(FRAME, COLOR);

	shape.SetFrame({ 10, 20, 30, 40 });

	EXPECT_EQ(shape.GetFrame(), (Frame{ 10, 20, 30, 40 }));
}

TEST_F(ShapeTest, CloneKeepsConcreteType)
{
	EXPECT_EQ(Rectangle(FRAME, COLOR).Clone()->GetType(), ShapeType::Rectangle);
	EXPECT_EQ(Ellipse(FRAME, COLOR).Clone()->GetType(), ShapeType::Ellipse);
	EXPECT_EQ(Triangle(FRAME, COLOR).Clone()->GetType(), ShapeType::Triangle);
}

TEST_F(ShapeTest, CloneCopiesFrameAndColor)
{
	const auto clone = Triangle(FRAME, COLOR).Clone();

	EXPECT_EQ(clone->GetFrame(), FRAME);
	EXPECT_EQ(clone->GetColor(), COLOR);
}

TEST_F(ShapeTest, CloneIsIndependentFromOriginal)
{
	const Triangle original(FRAME, COLOR);
	const auto clone = original.Clone();

	clone->SetFrame({ 10, 20, 30, 40 });

	EXPECT_EQ(original.GetFrame(), FRAME);
}
