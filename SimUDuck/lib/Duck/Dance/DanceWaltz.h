//
// Created by Вадим Патрушев on 10.09.2026.
//

#ifndef OOD_DANCEWALTZ_H
#define OOD_DANCEWALTZ_H
#include "IDanceBehavior.h"

#include <iostream>

class DanceWaltz : public IDanceBehavior
{
public:
	void Dance() const override
	{
		std::cout << "I'm dancing waltz!!" << std::endl;
	}
};

#endif // OOD_DANCEWALTZ_H
