//
// Created by Вадим Патрушев on 13.09.2026.
//

#ifndef OOD_DANCEBEHAVIOR_H
#define OOD_DANCEBEHAVIOR_H
#include <functional>
#include <iostream>

using DanceBehavior = std::function<void()>;

inline DanceBehavior DanceWaltz()
{
	return [] {
		std::cout << "I'm dancing waltz!!" << std::endl;
	};
}

inline DanceBehavior DanceMinuet()
{
	return [] {
		std::cout << "I'm dancing minuet!!" << std::endl;
	};
}

inline DanceBehavior DanceNoWay()
{
	return [] {};
}

#endif // OOD_DANCEBEHAVIOR_H
