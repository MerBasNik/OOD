//
// Created by Вадим Патрушев on 11.10.2026.
//

#include "CircleBehavior.h"
#include "IShape.h"
#include "LineBehavior.h"
#include "RectangleBehavior.h"
#include "Shape.h"
#include "TextBehavior.h"
#include "TriangleBehavior.h"
#include <gtest/gtest.h>

class ObserverShapeTest : public ::testing::Test
{
public:
	ShapeObserver observer;
	ShapeObserver observer2;

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

TEST_F(ObserverShapeTest, CreateCircle)
{
	const auto shape = CreateCircle("sh1", { 0, 0 }, 10, 0x000000);

	shape->AddObserver(observer);

	EXPECT_EQ(observer.GetCountCalls(), 0);
}

TEST_F(ObserverShapeTest, ChangeColor)
{
	const auto shape = CreateCircle("sh1", { 0, 0 }, 10, 0x000000);

	shape->AddObserver(observer);
	shape->ChangeColor(0xffffff);

	EXPECT_EQ(observer.GetCountCalls(), 1);
}

TEST_F(ObserverShapeTest, MoveShape)
{
	const auto shape = CreateCircle("sh1", { 0, 0 }, 10, 0x000000);

	shape->AddObserver(observer);
	shape->Move({ 100, 50 });

	EXPECT_EQ(observer.GetCountCalls(), 1);
}

TEST_F(ObserverShapeTest, ChangeBehavior)
{
	const auto shape = CreateCircle("sh1", { 0, 0 }, 10, 0x000000);

	shape->AddObserver(observer);
	shape->ChangeBehavior(std::make_unique<TextBehavior>(Point{ 0, 0 }, 0, "text"));

	EXPECT_EQ(observer.GetCountCalls(), 1);
}

TEST_F(ObserverShapeTest, GetColor)
{
	const auto shape = CreateCircle("sh1", { 0, 0 }, 10, 0x000000);

	shape->AddObserver(observer);
	const auto _ = shape->GetColor();

	EXPECT_EQ(observer.GetCountCalls(), 0);
}

TEST_F(ObserverShapeTest, SetCurrentColor)
{
	const auto shape = CreateCircle("sh1", { 0, 0 }, 10, 0x000000);

	shape->AddObserver(observer);
	shape->ChangeColor(0x000000);

	EXPECT_EQ(observer.GetCountCalls(), 1);
}

TEST_F(ObserverShapeTest, DoNotMoveShape)
{
	const auto shape = CreateCircle("sh1", { 0, 0 }, 10, 0x000000);

	shape->AddObserver(observer);
	shape->Move({ 0, 0 });

	EXPECT_EQ(observer.GetCountCalls(), 1);
}

TEST_F(ObserverShapeTest, SetCurrentBehavior)
{
	const auto shape = CreateCircle("sh1", { 0, 0 }, 10, 0x000000);

	shape->AddObserver(observer);
	shape->ChangeBehavior(std::make_unique<CircleBehavior>(Point{ 0, 0 }, 0));

	EXPECT_EQ(observer.GetCountCalls(), 1);
}

TEST_F(ObserverShapeTest, SetCurrentColorManyObservers)
{
	const auto shape = CreateCircle("sh1", { 0, 0 }, 10, 0x000000);

	shape->AddObserver(observer);
	shape->AddObserver(observer2);
	shape->ChangeColor(0xffffff);

	EXPECT_EQ(observer.GetCountCalls(), 1);
	EXPECT_EQ(observer2.GetCountCalls(), 1);
}

TEST_F(ObserverShapeTest, GetColorManyObservers)
{
	const auto shape = CreateCircle("sh1", { 0, 0 }, 10, 0x000000);

	shape->AddObserver(observer);
	shape->AddObserver(observer2);
	const auto _ = shape->GetColor();

	EXPECT_EQ(observer.GetCountCalls(), 0);
	EXPECT_EQ(observer2.GetCountCalls(), 0);
}

TEST_F(ObserverShapeTest, DublicateObserver)
{
	const auto shape = CreateCircle("sh1", { 0, 0 }, 10, 0x000000);

	shape->AddObserver(observer);
	shape->AddObserver(observer);
	shape->ChangeColor(0xffffff);

	EXPECT_EQ(observer.GetCountCalls(), 1);
}

TEST_F(ObserverShapeTest, DeleteObserverAfterAction)
{
	const auto shape = CreateCircle("sh1", { 0, 0 }, 10, 0x000000);

	shape->AddObserver(observer);
	shape->ChangeColor(0xffffff);
	shape->DeleteObserver(observer);

	EXPECT_EQ(observer.GetCountCalls(), 1);
}

TEST_F(ObserverShapeTest, DeleteObserverBeforeAction)
{
	const auto shape = CreateCircle("sh1", { 0, 0 }, 10, 0x000000);

	shape->AddObserver(observer);
	shape->DeleteObserver(observer);
	shape->ChangeColor(0xffffff);

	EXPECT_EQ(observer.GetCountCalls(), 0);
}
