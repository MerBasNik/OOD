//
// Created by Вадим Патрушев on 10.09.2026.
//

#ifndef OOD_FLYMOCK_H
#define OOD_FLYMOCK_H
#include "IFlyBehavior.h"

#include <gmock/gmock.h>

class FlyMock : public IFlyBehavior
{
public:
	MOCK_METHOD(void, Fly, (), (override));
};

#endif // OOD_FLYMOCK_H
