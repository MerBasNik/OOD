//
// Created by Вадим Патрушев on 12.09.2026.
//

#ifndef OOD_FLIESCOUNTER_H
#define OOD_FLIESCOUNTER_H
#include "IFlyBehavior.h"
#include "IFlyAction.h"

#include <iostream>
#include <memory>

class FliesCounter : public IFlyBehavior
{
public:
	explicit FliesCounter(
		std::unique_ptr<IFlyBehavior>&& flyBehavior,
		IFlyAction* flyAction)
		: m_flyBehavior(std::move(flyBehavior))
		, m_flyAction(flyAction)
	{
	}

	void Fly() override
	{
		if (NeedDoAction())
		{
			m_flyAction->FlyAction();
		}
		m_flyBehavior->Fly();
		m_fliesCount++;
		std::cout << "Fly number: " << m_fliesCount << std::endl;
	}

	bool CanFly() const override
	{
		return true;
	}

private:
	std::unique_ptr<IFlyBehavior> m_flyBehavior;
	IFlyAction* m_flyAction;
	int m_fliesCount = 0;

	bool NeedDoAction() const
	{
		return m_fliesCount % 2 != 0;
	};
};

#endif // OOD_FLIESCOUNTER_H
