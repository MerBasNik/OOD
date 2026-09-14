//
// Created by Вадим Патрушев on 13.09.2026.
//

#ifndef OOD_FLYMOCK_H
#define OOD_FLYMOCK_H
#include "IFlyBehavior.h"

#include <gmock/gmock.h>

class FlyMock : public IFlyBehavior
{
public:
	void Fly() override
	{
		m_count++;
	}

	bool CanFly() const override
	{
		return true;
	}

	int GetCount() const
	{
		return m_count;
	}

private:
	mutable int m_count = 0;
};

#endif // OOD_FLYMOCK_H
