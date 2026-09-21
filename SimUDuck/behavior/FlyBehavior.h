//
// Created by Вадим Патрушев on 13.09.2026.
//

#ifndef OOD_FLYBEHAVIOR_H
#define OOD_FLYBEHAVIOR_H
#include <functional>
#include <iostream>
#include <ostream>

using FlyBehavior = std::function<int()>;

inline FlyBehavior FlyWithWings()
{
	int fliesCount = 0;
	return [fliesCount]() mutable {
		fliesCount++;
		std::cout << "I'm flying with wings!!" << std::endl;
		return fliesCount;
	};
}

inline FlyBehavior FlyNoWay()
{
	return []() {
		return 0;
	};
}

#endif // OOD_FLYBEHAVIOR_H
