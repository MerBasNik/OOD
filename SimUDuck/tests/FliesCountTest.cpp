//
// Created by Вадим Патрушев on 13.09.2026.
//

#include "DanceWaltz.h"
#include "Duck.h"
#include "FlyMock.h"
#include "QuackMock.h"

#include <gtest/gtest.h>

class TestDuck : public Duck
{
public:
	using Duck::Duck;
	void Display() const override {};
};

TEST(TestMockFliesCount, OneFlyNoQuack)
{
	auto mockFly = std::make_unique<FlyMock>();
	auto mockQuack = std::make_unique<QuackMock>();

	const auto* fly = mockFly.get();
	const auto* quack = mockQuack.get();

	const TestDuck duck(
		std::move(mockFly),
		std::move(mockQuack),
		std::make_unique<DanceWaltz>());

	duck.Fly();

	EXPECT_EQ(fly->GetCount(), 1);
	EXPECT_EQ(quack->GetCount(), 0);
}

TEST(TestMockFliesCount, TwoFliesOneQuack)
{
	auto mockFly = std::make_unique<FlyMock>();
	auto mockQuack = std::make_unique<QuackMock>();

	const auto* fly = mockFly.get();
	const auto* quack = mockQuack.get();

	const TestDuck duck(
		std::move(mockFly),
		std::move(mockQuack),
		std::make_unique<DanceWaltz>());

	duck.Fly();
	duck.Fly();

	EXPECT_EQ(fly->GetCount(), 2);
	EXPECT_EQ(quack->GetCount(), 1);
}

TEST(TestMockFliesCount, ThreeFliesOneQuack)
{
	auto mockFly = std::make_unique<FlyMock>();
	auto mockQuack = std::make_unique<QuackMock>();

	const auto* fly = mockFly.get();
	const auto* quack = mockQuack.get();

	const TestDuck duck(
		std::move(mockFly),
		std::move(mockQuack),
		std::make_unique<DanceWaltz>());

	duck.Fly();
	duck.Fly();
	duck.Fly();

	EXPECT_EQ(fly->GetCount(), 3);
	EXPECT_EQ(quack->GetCount(), 1);
}

TEST(TestMockFliesCount, FourFliesTwoQuack)
{
	auto mockFly = std::make_unique<FlyMock>();
	auto mockQuack = std::make_unique<QuackMock>();

	const auto* fly = mockFly.get();
	const auto* quack = mockQuack.get();

	const TestDuck duck(
		std::move(mockFly),
		std::move(mockQuack),
		std::make_unique<DanceWaltz>());

	duck.Fly();
	duck.Fly();
	duck.Fly();
	duck.Fly();

	EXPECT_EQ(fly->GetCount(), 4);
	EXPECT_EQ(quack->GetCount(), 2);
}
