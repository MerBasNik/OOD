//
// Created by Вадим Патрушев on 14.09.2026.
//

#include "Duck.h"

inline Duck DecoyDuck()
{
	auto display = [] { std::cout << "I'm decoy duck" << std::endl; };
	return {
		FlyNoWay(),
		DanceNoWay(),
		MuteQuack(),
		display
	};
}

inline Duck MallardDuck()
{
	auto display = [] { std::cout << "I'm mallard duck" << std::endl; };
	return {
		FlyWithWings(),
		DanceWaltz(),
		Quack(),
		display
	};
}

inline Duck ModelDuck()
{
	auto display = [] { std::cout << "I'm model duck" << std::endl; };
	return {
		FlyNoWay(),
		DanceNoWay(),
		Quack(),
		display
	};
}

inline Duck RedheadDuck()
{
	auto display = [] { std::cout << "I'm redhead duck" << std::endl; };
	return {
		FlyWithWings(),
		DanceMinuet(),
		Quack(),
		display
	};
}

inline Duck RubberDuck()
{
	auto display = [] { std::cout << "I'm rubber duck" << std::endl; };
	return {
		FlyNoWay(),
		DanceNoWay(),
		SqueakQuack(),
		display
	};
}