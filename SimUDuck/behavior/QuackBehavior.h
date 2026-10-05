//
// Created by Вадим Патрушев on 13.09.2026.
//

#ifndef OOD_QUACKBEHAVIOR_H
#define OOD_QUACKBEHAVIOR_H
#include <functional>
#include <iostream>

using QuackBehavior = std::function<void()>;

inline QuackBehavior SqueakQuack()
{
	return [] {
		std::cout << "Squeek!!!" << std::endl;
	};
};

inline QuackBehavior MuteQuack()
{
	return [] {};
};

inline QuackBehavior Quack()
{
	return [] {
		std::cout << "Quack Quack!!!" << std::endl;
	};
};

#endif // OOD_QUACKBEHAVIOR_H
