#ifndef SQUEAKBEHAVIOR_H
#define SQUEAKBEHAVIOR_H

#include "IQuackBehavior.h"
#include <iostream>

class SqueakBehavior : public IQuackBehavior
{
public:
	void Quack() const override
	{
		std::cout << "Squeek!!!" << std::endl;
	}
};

#endif
