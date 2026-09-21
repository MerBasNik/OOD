//
// Created by Вадим Патрушев on 13.09.2026.
//

#ifndef OOD_QUACKMOCK_H
#define OOD_QUACKMOCK_H
#include "IQuackBehavior.h"

#include <gmock/gmock.h>

class QuackMock : public IQuackBehavior
{
public:
	MOCK_METHOD(void, Quack, (), (override));
};

#endif // OOD_QUACKMOCK_H
