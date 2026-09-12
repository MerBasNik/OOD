//
// Created by Вадим Патрушев on 12.09.2026.
//

#ifndef OOD_FLIESCOUNTER_H
#define OOD_FLIESCOUNTER_H
#include "IFlyBehavior.h"

#include <iostream>
#include <memory>

class FliesCounter : public IFlyBehavior
{
public:
	explicit FliesCounter(
		std::unique_ptr<IFlyBehavior>&& flyBehavior,
		IFlyObserver* flyObserver)
		: m_flyBehavior(std::move(flyBehavior)),
			m_flyObserver(flyObserver)
	{
	}

	void Fly() override
	{
		if (NeedDoObserverAction())
		{
			m_flyObserver->FlyAction();
		}
		m_flyBehavior->Fly();
		m_fliesCount++;
		std::cout << "Fly count: " << m_fliesCount << std::endl;
	}

	bool CanFly() override
	{
		return true;
	}
private:
	std::unique_ptr<IFlyBehavior> m_flyBehavior;
	IFlyObserver* m_flyObserver;
	int m_fliesCount = 0;

	bool NeedDoObserverAction() const
	{
		return m_fliesCount % 2 != 0;
	};
};

#endif //OOD_FLIESCOUNTER_H
