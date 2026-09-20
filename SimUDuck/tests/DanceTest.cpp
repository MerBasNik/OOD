//
// Created by Вадим Патрушев on 10.09.2026.
//

#include "DanceMock.h"
#include "MallardDuck.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

class TestDuck : public Duck
{
public:
	using Duck::Duck;
	void Display() const override {};
};

TEST(TestMockDance, NoDance)
{
	auto mockDance = std::make_unique<DanceMock>();
	const DanceMock* dance = mockDance.get();

	const TestDuck duck(
		std::make_unique<FlyWithWings>(),
		std::make_unique<QuackBehavior>(),
		std::move(mockDance));

	duck.Quack();

	EXPECT_CALL(*dance, Dance()).Times(0);
}

TEST(TestMockDance, DanceOneTime)
{
	auto mockDance = std::make_unique<DanceMock>();
	const DanceMock* dance = mockDance.get();

	const TestDuck duck(
		std::make_unique<FlyWithWings>(),
		std::make_unique<QuackBehavior>(),
		std::move(mockDance));

	duck.Dance();

	EXPECT_CALL(*dance, Dance()).Times(1);
}

TEST(TestMockDance, DanceTwoTime)
{
	auto mockDance = std::make_unique<DanceMock>();
	const DanceMock* dance = mockDance.get();

	const TestDuck duck(
		std::make_unique<FlyWithWings>(),
		std::make_unique<QuackBehavior>(),
		std::move(mockDance));

	duck.Dance();
	duck.Dance();

	EXPECT_CALL(*dance, Dance()).Times(2);
}
