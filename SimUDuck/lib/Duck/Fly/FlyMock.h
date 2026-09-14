//
// Created by Вадим Патрушев on 10.09.2026.
//

#ifndef OOD_FLYMOCK_H
#define OOD_FLYMOCK_H
#include "IFlyBehavior.h"

class FlyMock : public IFlyBehavior
{
public:
	void Fly() const
	{
		m_count++;
	}

	void IncrementCount() const
	{
		m_count++;
	}

	int GetCount() const override
	{
		return m_count;
	}
private:
	mutable int m_count = 0;
};

#endif // OOD_FLYMOCK_H
