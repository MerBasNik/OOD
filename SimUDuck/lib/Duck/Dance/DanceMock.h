//
// Created by Вадим Патрушев on 10.09.2026.
//

#ifndef OOD_DANCEMOCK_H
#define OOD_DANCEMOCK_H
#include "IDanceBehavior.h"

#include <gmock/gmock.h>

class DanceMock : public IDanceBehavior
{
public:
	MOCK_METHOD(void, Dance, (), (const, override));
};

#endif // OOD_DANCEMOCK_H
