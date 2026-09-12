//
// Created by Вадим Патрушев on 10.09.2026.
//

#ifndef OOD_DANCEMINUET_H
#define OOD_DANCEMINUET_H
#include "IDanceBehavior.h"

#include <iostream>

class DanceMinuet : public IDanceBehavior
{
public:
	void Dance() const override
	{
		std::cout << "I'm dancing minuet!!" << std::endl;
	}
};

#endif // OOD_DANCEMINUET_H
