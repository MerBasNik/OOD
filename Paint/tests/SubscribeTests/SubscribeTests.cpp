//
// Created by Вадим Патрушев on 12.10.2026.
//

#include "CircleBehavior.h"
#include "LineBehavior.h"
#include "Picture.h"
#include "PictureObserverSub.h"
#include "PictureObserverUnsub.h"
#include "PictureObserverUnsubAndSub.h"
#include "Shape.h"
#include <gtest/gtest.h>

class SubscribeTest : public ::testing::Test
{
public:
	Picture picture;

	void AddCircle(const std::string& id, const double x, const double y, const double r)
	{
		picture.AddShape(std::make_unique<Shape>(id, 0x000000, std::make_unique<CircleBehavior>(Point{ x, y }, r)));
	}

	void AddLine(const std::string& id, const double x1, const double y1, const double x2, const double y2)
	{
		picture.AddShape(std::make_unique<Shape>(id, 0x000000, std::make_unique<LineBehavior>(Point{ x1, y1 }, Point{ x2, y2 })));
	}
};

TEST_F(SubscribeTest, UnsubscribeWhileDoAction)
{
	PictureObserver observerB;
	PictureObserver observerC;
	PictureObserverUnsub observerA(observerB, &picture);
	picture.AddObserver(observerA);
	picture.AddObserver(observerB);
	picture.AddObserver(observerC);

	AddCircle("sh1", 0, 0, 10);
	picture.ChangeColor("sh1", 0xffffff);

	EXPECT_EQ(observerA.GetCountCalls(), 2);
	EXPECT_EQ(observerB.GetCountCalls(), 0);
	EXPECT_EQ(observerC.GetCountCalls(), 2);
}

TEST_F(SubscribeTest, SubscribeWhileDoAction)
{
	PictureObserver observerB;
	PictureObserver observerC;
	PictureObserverSub observerA(observerC, &picture);
	picture.AddObserver(observerA);
	picture.AddObserver(observerB);

	AddCircle("sh1", 0, 0, 10);
	picture.ChangeColor("sh1", 0xffffff);

	EXPECT_EQ(observerA.GetCountCalls(), 2);
	EXPECT_EQ(observerB.GetCountCalls(), 2);
	EXPECT_EQ(observerC.GetCountCalls(), 1);

	picture.ChangeColor("sh1", 0xffffff);

	EXPECT_EQ(observerA.GetCountCalls(), 3);
	EXPECT_EQ(observerB.GetCountCalls(), 3);
	EXPECT_EQ(observerC.GetCountCalls(), 2);
}

TEST_F(SubscribeTest, UnsubscribeAndSubscribeWhileDoAction)
{
	PictureObserver observerB;
	PictureObserver observerC;
	PictureObserverUnsubAndSub observerA(observerB, &picture);
	picture.AddObserver(observerA);
	picture.AddObserver(observerB);
	picture.AddObserver(observerC);

	AddCircle("sh1", 0, 0, 10);
	picture.ChangeColor("sh1", 0xffffff);

	EXPECT_EQ(observerA.GetCountCalls(), 2);
	EXPECT_EQ(observerB.GetCountCalls(), 0);
	EXPECT_EQ(observerC.GetCountCalls(), 2);

	picture.DeleteObserver(observerA);
	picture.ChangeColor("sh1", 0xffffff);

	EXPECT_EQ(observerB.GetCountCalls(), 1);
	EXPECT_EQ(observerC.GetCountCalls(), 3);
}