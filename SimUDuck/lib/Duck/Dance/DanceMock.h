//
// Created by Вадим Патрушев on 10.09.2026.
//

#ifndef OOD_DANCEMOCK_H
#define OOD_DANCEMOCK_H
#include "IDanceBehavior.h"

class DanceMock : public IDanceBehavior
{
public:
	void Dance() const override
	{
		m_count++;
	}

	int GetCount() const
	{
		return m_count;
	}

private:
	mutable int m_count = 0;
};

#endif // OOD_DANCEMOCK_H
