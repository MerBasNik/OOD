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

TEST(TestMockDance, DanceOneTime)
{
	auto mockDance = std::make_unique<DanceMock>();
	EXPECT_CALL(*mockDance, Dance()).Times(1);

	const TestDuck duck(
		std::make_unique<FlyWithWings>(),
		std::make_unique<QuackBehavior>(),
		std::move(mockDance));

	duck.Dance();
}

TEST(TestMockDance, DanceTwoTime)
{
	auto mockDance = std::make_unique<DanceMock>();
	EXPECT_CALL(*mockDance, Dance()).Times(2);

	const TestDuck duck(
		std::make_unique<FlyWithWings>(),
		std::make_unique<QuackBehavior>(),
		std::move(mockDance));

	duck.Dance();
	duck.Dance();
}
