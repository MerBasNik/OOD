//
// Created by Вадим Патрушев on 13.09.2026.
//

#ifndef OOD_QUACKMOCK_H
#define OOD_QUACKMOCK_H
#include "IQuackBehavior.h"

class QuackMock : public IQuackBehavior
{
public:
	void Quack() const override
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

#endif // OOD_QUACKMOCK_H
