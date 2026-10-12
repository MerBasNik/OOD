//
// Created by Вадим Патрушев on 11.10.2026.
//

#include "CircleBehavior.h"
#include "LineBehavior.h"
#include "Picture.h"
#include "Shape.h"
#include <gtest/gtest.h>

class ObserverPictureTest : public ::testing::Test
{
public:
	Picture picture;
	PictureObserver observer;
	PictureObserver observer2;

	void AddCircle(const std::string& id, const double x, const double y, const double r)
	{
		picture.AddShape(std::make_unique<Shape>(id, 0x000000, std::make_unique<CircleBehavior>(Point{ x, y }, r)));
	}

	void AddLine(const std::string& id, const double x1, const double y1, const double x2, const double y2)
	{
		picture.AddShape(std::make_unique<Shape>(id, 0x000000, std::make_unique<LineBehavior>(Point{ x1, y1 }, Point{ x2, y2 })));
	}
};

TEST_F(ObserverPictureTest, AddShape)
{
	picture.AddObserver(observer);

	AddCircle("sh1", 0, 0, 10);

	EXPECT_EQ(observer.GetCountCalls(), 1);
}

TEST_F(ObserverPictureTest, DeleteShape)
{
	picture.AddObserver(observer);
	AddCircle("sh1", 0, 0, 10);

	picture.DeleteShape("sh1");

	EXPECT_EQ(observer.GetCountCalls(), 2);
}

TEST_F(ObserverPictureTest, SetShapeColor)
{
	picture.AddObserver(observer);
	AddCircle("sh1", 0, 0, 10);

	picture.ChangeColor("sh1", 0xffffff);

	EXPECT_EQ(observer.GetCountCalls(), 2);
}

TEST_F(ObserverPictureTest, MoveShape)
{
	picture.AddObserver(observer);
	AddCircle("sh1", 0, 0, 10);

	picture.MoveShape("sh1", Point{ 10, 10 });

	EXPECT_EQ(observer.GetCountCalls(), 2);
}

TEST_F(ObserverPictureTest, SetShapeColorFromPtr)
{
	picture.AddObserver(observer);
	AddCircle("sh1", 0, 0, 10);

	auto* shape = picture.GetShape("sh1");
	shape->ChangeColor(0xffffff);

	EXPECT_EQ(observer.GetCountCalls(), 2);
}

TEST_F(ObserverPictureTest, MoveShapeFromPtr)
{
	picture.AddObserver(observer);
	AddCircle("sh1", 0, 0, 10);

	auto* shape = picture.GetShape("sh1");
	shape->Move(Point{ 10, 10 });

	EXPECT_EQ(observer.GetCountCalls(), 2);
}

TEST_F(ObserverPictureTest, DublicateObserver)
{
	picture.AddObserver(observer);
	picture.AddObserver(observer);

	AddCircle("sh1", 0, 0, 10);

	EXPECT_EQ(observer.GetCountCalls(), 1);
}

TEST_F(ObserverPictureTest, DeleteObserverAfterAction)
{
	picture.AddObserver(observer);

	AddCircle("sh1", 0, 0, 10);
	picture.DeleteObserver(observer);

	EXPECT_EQ(observer.GetCountCalls(), 0);
}

TEST_F(ObserverPictureTest, DeleteObserverBeforeAction)
{
	picture.AddObserver(observer);

	picture.DeleteObserver(observer);
	AddCircle("sh1", 0, 0, 10);

	EXPECT_EQ(observer.GetCountCalls(), 0);
}

TEST_F(ObserverPictureTest, DeleteShapeFromPicture)
{
	picture.AddObserver(observer);

	AddCircle("sh1", 0, 0, 10);
	picture.DeleteShape("sh1");
	picture.ChangeColor("sh1", 0xffffff);

	EXPECT_EQ(observer.GetCountCalls(), 2);
}
