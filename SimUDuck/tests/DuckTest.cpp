//
// Created by Вадим Патрушев on 10.09.2026.
//

#include "Duck.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(TestDance, NoWayDance)
{
	const int danceCount = 0;
	DanceBehavior mockDance = [] {};

	const Duck duck(
		FlyNoWay(),
		std::move(mockDance),
		MuteQuack(),
		[] {});

	duck.Dance();

	EXPECT_EQ(danceCount, 0);
}

TEST(TestDance, NoDance)
{
	int danceCount = 0;
	DanceBehavior mockDance = [&danceCount] { danceCount++; };

	Duck duck(
		FlyNoWay(),
		std::move(mockDance),
		MuteQuack(),
		[] {});

	EXPECT_EQ(danceCount, 0);
}

TEST(TestDance, DanceOneTime)
{
	int danceCount = 0;
	DanceBehavior mockDance = [&danceCount] { danceCount++; };

	const Duck duck(
		FlyNoWay(),
		std::move(mockDance),
		MuteQuack(),
		[] {});

	duck.Dance();

	EXPECT_EQ(danceCount, 1);
}

TEST(TestDance, DanceTwoTimes)
{
	int danceCount = 0;
	DanceBehavior mockDance = [&danceCount] { danceCount++; };

	const Duck duck(
		FlyNoWay(),
		std::move(mockDance),
		MuteQuack(),
		[] {});

	duck.Dance();
	duck.Dance();

	EXPECT_EQ(danceCount, 2);
}

TEST(TestFly, NoWayFly)
{
	const int fliesCount = 0;
	FlyBehavior flyBehavior = [] {};

	Duck duck(
		std::move(flyBehavior),
		DanceNoWay(),
		MuteQuack(),
		[] {});

	EXPECT_EQ(fliesCount, 0);
}

TEST(TestFly, NoFly)
{
	int fliesCount = 0;
	FlyBehavior flyBehavior = [&fliesCount] { fliesCount++; };

	Duck duck(
		std::move(flyBehavior),
		DanceNoWay(),
		MuteQuack(),
		[] {});

	EXPECT_EQ(fliesCount, 0);
}

TEST(TestFly, FlyOneTime)
{
	int fliesCount = 0;
	FlyBehavior flyBehavior = [&fliesCount] { fliesCount++; };

	const Duck duck(
		std::move(flyBehavior),
		DanceNoWay(),
		MuteQuack(),
		[] {});

	duck.Fly();

	EXPECT_EQ(fliesCount, 1);
}

TEST(TestFly, FlyTwoTimes)
{
	int fliesCount = 0;
	FlyBehavior flyBehavior = [&fliesCount] { fliesCount++; };

	const Duck duck(
		std::move(flyBehavior),
		DanceNoWay(),
		MuteQuack(),
		[] {});

	duck.Fly();
	duck.Fly();

	EXPECT_EQ(fliesCount, 2);
}

TEST(TestQuack, NoWayQuack)
{
	const int quackCount = 0;
	QuackBehavior quackBehavior = [] {};

	const Duck duck(
		FlyNoWay(),
		DanceNoWay(),
		std::move(quackBehavior),
		[] {});

	duck.Quack();

	EXPECT_EQ(quackCount, 0);
}

TEST(TestQuack, NoQuack)
{
	int quackCount = 0;
	QuackBehavior quackBehavior = [&quackCount] { quackCount++; };

	const Duck duck(
		FlyNoWay(),
		DanceNoWay(),
		std::move(quackBehavior),
		[] {});

	EXPECT_EQ(quackCount, 0);
}

TEST(TestQuack, QuackOneTime)
{
	int quackCount = 0;
	QuackBehavior quackBehavior = [&quackCount] { quackCount++; };

	const Duck duck(
		FlyNoWay(),
		DanceNoWay(),
		std::move(quackBehavior),
		[] {});

	duck.Quack();

	EXPECT_EQ(quackCount, 1);
}

TEST(TestQuack, QuackTwoTimes)
{
	int quackCount = 0;
	QuackBehavior quackBehavior = [&quackCount] { quackCount++; };

	const Duck duck(
		FlyNoWay(),
		DanceNoWay(),
		std::move(quackBehavior),
		[] {});

	duck.Quack();
	duck.Quack();

	EXPECT_EQ(quackCount, 2);
}
