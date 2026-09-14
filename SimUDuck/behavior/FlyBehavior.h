//
// Created by Вадим Патрушев on 13.09.2026.
//

#ifndef OOD_FLYBEHAVIOR_H
#define OOD_FLYBEHAVIOR_H
#include <functional>
#include <iostream>
#include <ostream>

using FlyBehavior = std::function<void()>;

inline FlyBehavior FlyWithWings()
{
	return [] {
		std::cout << "I'm flying with wings!!" << std::endl;
	};
}

inline FlyBehavior FlyNoWay()
{
	return [] {};
}

inline FlyBehavior FliesCounter(
	FlyBehavior flyBehavior,
	std::function<void()> flyAction = {})
{
	return [flyBehavior, flyAction, fliesCount = 0]() mutable {
		const bool needDoAction = fliesCount % 2 != 0;
		if (needDoAction)
		{
			flyAction();
		}
		flyBehavior();
		fliesCount++;
		std::cout << "Fly number: " << fliesCount << std::endl;
	};
}

#endif // OOD_FLYBEHAVIOR_H
