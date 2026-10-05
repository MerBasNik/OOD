//
// Created by Вадим Патрушев on 05.10.2026.
//

#include "CircleBehavior.h"
#include "LineBehavior.h"
#include "MockCanvas.h"
#include "Picture.h"
#include "RectangleBehavior.h"
#include "Shape.h"
#include "TextBehavior.h"
#include "TriangleBehavior.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::_;

class ShapeTest : public ::testing::Test
{
public:
	static std::unique_ptr<IShape> CreateCircle(const std::string& id, Point center, double radius, Color color = 0)
	{
		return std::make_unique<Shape>(id, color, std::make_unique<CircleBehavior>(center, radius));
	}

	static std::unique_ptr<IShape> CreateRectangle(const std::string& id, Point topLeft, double width, double height, Color color = 0)
	{
		return std::make_unique<Shape>(id, color, std::make_unique<RectangleBehavior>(topLeft, width, height));
	}

	static std::unique_ptr<IShape> CreateTriangle(const std::string& id, Point v1, Point v2, Point v3, Color color = 0)
	{
		return std::make_unique<Shape>(id, color, std::make_unique<TriangleBehavior>(v1, v2, v3));
	}

	static std::unique_ptr<IShape> CreateLine(const std::string& id, Point start, Point end, Color color = 0)
	{
		return std::make_unique<Shape>(id, color, std::make_unique<LineBehavior>(start, end));
	}

	static std::unique_ptr<IShape> CreateText(const std::string& id, Point position, double fontSize, const std::string& text, Color color = 0)
	{
		return std::make_unique<Shape>(id, color, std::make_unique<TextBehavior>(position, fontSize, text));
	}
};

TEST_F(ShapeTest, TestCreateCircle)
{
	const auto shape = CreateCircle("sh1", { 0, 0 }, 10, 0x000000);

	const std::string expected = "0 0 10";

	EXPECT_EQ(shape->GetInfo(), expected);
	EXPECT_EQ(shape->GetName(), "circle");
	EXPECT_EQ(shape->GetId(), "sh1");
	EXPECT_EQ(shape->GetColor(), 0x000000);
}

TEST_F(ShapeTest, TestCreateRectangle)
{
	const auto shape = CreateRectangle("sh1", { 0, 0 }, 100, 200, 0x000000);

	const std::string expected = "0 0";

	EXPECT_EQ(shape->GetInfo(), expected);
	EXPECT_EQ(shape->GetName(), "rectangle");
	EXPECT_EQ(shape->GetId(), "sh1");
	EXPECT_EQ(shape->GetColor(), 0x000000);
}

TEST_F(ShapeTest, TestCreateTriangle)
{
	const auto shape = CreateTriangle("sh1", { 0, 0 }, { 10, 10 }, { 10, 0 }, 0x000000);

	const std::string expected = "0 0 10 10 10 0";

	EXPECT_EQ(shape->GetInfo(), expected);
	EXPECT_EQ(shape->GetName(), "triangle");
	EXPECT_EQ(shape->GetId(), "sh1");
	EXPECT_EQ(shape->GetColor(), 0x000000);
}

TEST_F(ShapeTest, TestCreateLine)
{
	const auto shape = CreateLine("sh1", { 0, 0 }, { 10, 10 }, 0x000000);

	const std::string expected = "0 0 10 10";

	EXPECT_EQ(shape->GetInfo(), expected);
	EXPECT_EQ(shape->GetName(), "line");
	EXPECT_EQ(shape->GetId(), "sh1");
	EXPECT_EQ(shape->GetColor(), 0x000000);
}

TEST_F(ShapeTest, TestCreateText)
{
	const auto shape = CreateText("sh1", { 0, 0 }, 16, "Привет", 0x000000);

	const std::string expected = "0 0 16 Привет";

	EXPECT_EQ(shape->GetInfo(), expected);
	EXPECT_EQ(shape->GetName(), "text");
	EXPECT_EQ(shape->GetId(), "sh1");
	EXPECT_EQ(shape->GetColor(), 0x000000);
}

TEST_F(ShapeTest, ChangeColor)
{
	const auto shape = CreateCircle("sh1", { 0, 0 }, 10, 0x000000);

	shape->ChangeColor(0xffffff);

	EXPECT_EQ(shape->GetColor(), 0xffffff);
}

TEST_F(ShapeTest, SetNewId)
{
	const auto shape = CreateCircle("sh1", { 0, 0 }, 10, 0x000000);

	shape->SetId("sh2");

	EXPECT_EQ(shape->GetId(), "sh2");
}

TEST_F(ShapeTest, ChangeBehavior)
{
	const auto shape = CreateCircle("sh1", { 0, 0 }, 10, 0x000000);

	shape->ChangeBehavior(std::make_unique<RectangleBehavior>(Point{ 0, 0 }, 30, 10));

	EXPECT_EQ(shape->GetName(), "rectangle");
}

TEST_F(ShapeTest, CircleDraw)
{
	MockCanvas canvas;
	const auto shape = CreateCircle("sh1", { 100, 200 }, 50, 0x000000);

	EXPECT_CALL(canvas, SetColor(0x000000));
	EXPECT_CALL(canvas, DrawEllipse(Point{ 100, 200 }, Point{ 50, 50 }));

	shape->Draw(canvas);
}

TEST_F(ShapeTest, RectangleDraw)
{
	MockCanvas canvas;
	const auto shape = CreateRectangle("sh1", { 0, 0 }, 100, 50, 0x000000);

	EXPECT_CALL(canvas, SetColor(0x000000));
	EXPECT_CALL(canvas, MoveTo(Point{ 0, 0 }));
	EXPECT_CALL(canvas, LineTo(Point{ 100, 0 }));
	EXPECT_CALL(canvas, LineTo(Point{ 100, 50 }));
	EXPECT_CALL(canvas, LineTo(Point{ 0, 50 }));
	EXPECT_CALL(canvas, LineTo(Point{ 0, 0 }));

	shape->Draw(canvas);
}

TEST_F(ShapeTest, TriangleDraw)
{
	MockCanvas canvas;
	const auto shape = CreateTriangle("sh1", { 0, 0 }, { 10, 10 }, { 10, 0 }, 0x000000);

	EXPECT_CALL(canvas, SetColor(0x000000));
	EXPECT_CALL(canvas, MoveTo(Point{ 0, 0 }));
	EXPECT_CALL(canvas, LineTo(Point{ 10, 10 }));
	EXPECT_CALL(canvas, LineTo(Point{ 10, 0 }));
	EXPECT_CALL(canvas, LineTo(Point{ 0, 0 }));

	shape->Draw(canvas);
}

TEST_F(ShapeTest, LineDraw)
{
	MockCanvas canvas;
	const auto shape = CreateLine("sh1", { 10, 20 }, { 30, 40 }, 0x000000);

	EXPECT_CALL(canvas, SetColor(0x000000));
	EXPECT_CALL(canvas, MoveTo(Point{ 10, 20 }));
	EXPECT_CALL(canvas, LineTo(Point{ 30, 40 }));

	shape->Draw(canvas);
}

TEST_F(ShapeTest, TextDraw)
{
	MockCanvas canvas;
	const auto shape = CreateText("sh1", { 50, 20 }, 16, "Привет", 0x000000);

	EXPECT_CALL(canvas, SetColor(0x000000));
	EXPECT_CALL(canvas, DrawText(Point{ 50, 20 }, 16, "Привет"));

	shape->Draw(canvas);
}

TEST_F(ShapeTest, MoveCircle)
{
	MockCanvas canvas;
	const auto shape = CreateCircle("sh1", { 10, 20 }, 5);

	shape->Move({ 7, -5 });

	EXPECT_CALL(canvas, SetColor(0x000000));
	EXPECT_CALL(canvas, DrawEllipse(Point{ 17, 15 }, Point{ 5, 5 }));

	shape->Draw(canvas);
}

TEST_F(ShapeTest, MoveRectangle)
{
	MockCanvas canvas;
	const auto shape = CreateRectangle("sh1", { 10, 20 }, 100, 50);

	shape->Move({ 7, -5 });

	EXPECT_CALL(canvas, SetColor(0x000000));
	EXPECT_CALL(canvas, MoveTo(Point{ 17, 15 }));
	EXPECT_CALL(canvas, LineTo(_)).Times(4);

	shape->Draw(canvas);
}

TEST_F(ShapeTest, MoveTriangle)
{
	MockCanvas canvas;
	const auto shape = CreateTriangle("sh1", { 0, 0 }, { 10, 10 }, { 10, 0 });

	shape->Move({ 7, -5 });

	EXPECT_CALL(canvas, SetColor(0x000000));
	EXPECT_CALL(canvas, MoveTo(Point{ 7, -5 }));
	EXPECT_CALL(canvas, LineTo(_)).Times(3);

	shape->Draw(canvas);
}

TEST_F(ShapeTest, MoveText)
{
	MockCanvas canvas;
	const auto shape = CreateText("sh1", { 50, 20 }, 16, "Привет");

	shape->Move({ 7, -5 });

	EXPECT_CALL(canvas, SetColor(0x000000));
	EXPECT_CALL(canvas, DrawText(Point{ 57, 15 }, 16, "Привет"));

	shape->Draw(canvas);
}
