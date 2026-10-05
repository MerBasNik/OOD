//
// Created by Вадим Патрушев on 13.09.2026.
//

#include "Duck.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(TestFlyAction, NoFlyOneQuack)
{
	int quackCount = 0;
	int fliesCount = 0;
	QuackBehavior quackBehavior = [&quackCount] { quackCount++; };
	FlyBehavior flyBehavior = [&fliesCount, quackBehavior] {
		if (fliesCount % 2 == 0)
		{
			quackBehavior();
		}
		fliesCount++;
	};

	const Duck duck(
		std::move(flyBehavior),
		DanceNoWay(),
		std::move(quackBehavior),
		[] {});

	duck.Quack();

	EXPECT_EQ(fliesCount, 0);
	EXPECT_EQ(quackCount, 1);
}

TEST(TestFlyAction, FlyOneTime)
{
	int quackCount = 0;
	int fliesCount = 0;
	QuackBehavior quackBehavior = [&quackCount] { quackCount++; };
	FlyBehavior flyBehavior = [&fliesCount, quackBehavior] {
		if (fliesCount % 2 == 0)
		{
			quackBehavior();
		}
		fliesCount++;
	};

	const Duck duck(
		std::move(flyBehavior),
		DanceNoWay(),
		std::move(quackBehavior),
		[] {});

	duck.Fly();

	EXPECT_EQ(fliesCount, 1);
	EXPECT_EQ(quackCount, 1);
}

TEST(TestFlyAction, FlyTwoTimes)
{
	int quackCount = 0;
	int fliesCount = 0;
	QuackBehavior quackBehavior = [&quackCount] { quackCount++; };
	FlyBehavior flyBehavior = [&fliesCount, quackBehavior] {
		if (fliesCount % 2 == 0)
		{
			quackBehavior();
		}
		fliesCount++;
	};

	const Duck duck(
		std::move(flyBehavior),
		DanceNoWay(),
		std::move(quackBehavior),
		[] {});

	duck.Fly();
	duck.Fly();

	EXPECT_EQ(fliesCount, 2);
	EXPECT_EQ(quackCount, 1);
}

TEST(TestFlyAction, FlyThreeTimes)
{
	int quackCount = 0;
	int fliesCount = 0;
	QuackBehavior quackBehavior = [&quackCount] { quackCount++; };
	FlyBehavior flyBehavior = [&fliesCount, quackBehavior] {
		if (fliesCount % 2 == 0)
		{
			quackBehavior();
		}
		fliesCount++;
	};

	const Duck duck(
		std::move(flyBehavior),
		DanceNoWay(),
		std::move(quackBehavior),
		[] {});

	duck.Fly();
	duck.Fly();
	duck.Fly();

	EXPECT_EQ(fliesCount, 3);
	EXPECT_EQ(quackCount, 2);
}