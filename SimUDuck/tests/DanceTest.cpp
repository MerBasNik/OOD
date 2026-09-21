//
// Created by Вадим Патрушев on 10.09.2026.
//

#include "DanceMock.h"
#include "MallardDuck.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

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
	EXPECT_CALL(*dance, Dance()).Times(0);

	duck.Quack();
}

TEST(TestMockDance, DanceOneTime)
{
	auto mockDance = std::make_unique<DanceMock>();
	const DanceMock* dance = mockDance.get();

	const TestDuck duck(
		std::make_unique<FlyWithWings>(),
		std::make_unique<QuackBehavior>(),
		std::move(mockDance));
	EXPECT_CALL(*dance, Dance()).Times(1);

	duck.Dance();
}

TEST(TestMockDance, DanceTwoTime)
{
	auto mockDance = std::make_unique<DanceMock>();
	const DanceMock* dance = mockDance.get();

	const TestDuck duck(
		std::make_unique<FlyWithWings>(),
		std::make_unique<QuackBehavior>(),
		std::move(mockDance));
	EXPECT_CALL(*dance, Dance()).Times(2);

	duck.Dance();
	duck.Dance();
}
