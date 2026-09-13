//
// Created by Вадим Патрушев on 13.09.2026.
//

#include "DanceWaltz.h"
#include "Duck.h"
#include "FlyMock.h"
#include "QuackBehavior.h"
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

	ON_CALL(*mockFly, CanFly()).WillByDefault(::testing::Return(true));

	EXPECT_CALL(*mockFly, Fly()).Times(1);
	EXPECT_CALL(*mockQuack, Quack()).Times(0);

	const TestDuck duck(
		std::move(mockFly),
		std::move(mockQuack),
		std::make_unique<DanceWaltz>());

	duck.Fly();
}

TEST(TestMockFliesCount, TwoFliesOneQuack)
{
	auto mockFly = std::make_unique<FlyMock>();
	auto mockQuack = std::make_unique<QuackMock>();

	ON_CALL(*mockFly, CanFly()).WillByDefault(::testing::Return(true));

	EXPECT_CALL(*mockFly, Fly()).Times(2);
	EXPECT_CALL(*mockQuack, Quack()).Times(1);

	const TestDuck duck(
		std::move(mockFly),
		std::move(mockQuack),
		std::make_unique<DanceWaltz>());

	duck.Fly();
	duck.Fly();
}

TEST(TestMockFliesCount, ThreeFliesOneQuack)
{
	auto mockFly = std::make_unique<FlyMock>();
	auto mockQuack = std::make_unique<QuackMock>();

	ON_CALL(*mockFly, CanFly()).WillByDefault(::testing::Return(true));

	EXPECT_CALL(*mockFly, Fly()).Times(3);
	EXPECT_CALL(*mockQuack, Quack()).Times(1);

	const TestDuck duck(
		std::move(mockFly),
		std::move(mockQuack),
		std::make_unique<DanceWaltz>());

	duck.Fly();
	duck.Fly();
	duck.Fly();
}

TEST(TestMockFliesCount, FourFliesTwoQuack)
{
	auto mockFly = std::make_unique<FlyMock>();
	auto mockQuack = std::make_unique<QuackMock>();

	ON_CALL(*mockFly, CanFly()).WillByDefault(::testing::Return(true));

	EXPECT_CALL(*mockFly, Fly()).Times(4);
	EXPECT_CALL(*mockQuack, Quack()).Times(2);

	const TestDuck duck(
		std::move(mockFly),
		std::move(mockQuack),
		std::make_unique<DanceWaltz>());

	duck.Fly();
	duck.Fly();
	duck.Fly();
	duck.Fly();
}
