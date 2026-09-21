//
// Created by Вадим Патрушев on 13.09.2026.
//

#include "DanceWaltz.h"
#include "Duck.h"
#include "FlyMock.h"
#include "QuackMock.h"

#include <gmock/gmock.h>
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

	auto* fly = mockFly.get();
	auto* quack = mockQuack.get();

	const TestDuck duck(
		std::move(mockFly),
		std::move(mockQuack),
		std::make_unique<DanceWaltz>());
	EXPECT_CALL(*fly, Fly()).Times(1);
	EXPECT_CALL(*quack, Quack()).Times(0);

	duck.Fly();
}

TEST(TestMockFliesCount, TwoFliesOneQuack)
{
	auto mockFly = std::make_unique<FlyMock>();
	auto mockQuack = std::make_unique<QuackMock>();

	auto* fly = mockFly.get();
	auto* quack = mockQuack.get();

	const TestDuck duck(
		std::move(mockFly),
		std::move(mockQuack),
		std::make_unique<DanceWaltz>());
	EXPECT_CALL(*fly, Fly()).Times(2);
	EXPECT_CALL(*quack, Quack()).Times(1);

	duck.Fly();
	duck.Fly();
}

TEST(TestMockFliesCount, ThreeFliesOneQuack)
{
	auto mockFly = std::make_unique<FlyMock>();
	auto mockQuack = std::make_unique<QuackMock>();

	auto* fly = mockFly.get();
	auto* quack = mockQuack.get();

	const TestDuck duck(
		std::move(mockFly),
		std::move(mockQuack),
		std::make_unique<DanceWaltz>());
	EXPECT_CALL(*fly, Fly()).Times(3);
	EXPECT_CALL(*quack, Quack()).Times(1);

	duck.Fly();
	duck.Fly();
	duck.Fly();
}

TEST(TestMockFliesCount, FourFliesTwoQuack)
{
	auto mockFly = std::make_unique<FlyMock>();
	auto mockQuack = std::make_unique<QuackMock>();

	auto* fly = mockFly.get();
	auto* quack = mockQuack.get();

	const TestDuck duck(
		std::move(mockFly),
		std::move(mockQuack),
		std::make_unique<DanceWaltz>());
	EXPECT_CALL(*fly, Fly()).Times(4);
	EXPECT_CALL(*quack, Quack()).Times(2);

	duck.Fly();
	duck.Fly();
	duck.Fly();
	duck.Fly();
}
