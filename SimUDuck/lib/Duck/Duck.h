#ifndef DUCK_H
#define DUCK_H

#include "Dance/IDanceBehavior.h"
#include "Fly/IFlyBehavior.h"
#include "IFlyAction.h"
#include "Quack/IQuackBehavior.h"

#include <cassert>
#include <iostream>
#include <memory>

class Duck : public IFlyAction
{
public:
	Duck(std::unique_ptr<IFlyBehavior>&& flyBehavior,
		std::unique_ptr<IQuackBehavior>&& quackBehavior,
		std::unique_ptr<IDanceBehavior>&& danceBehavior)
		: m_quackBehavior(std::move(quackBehavior))
		, m_danceBehavior(std::move(danceBehavior))
	{
		assert(m_quackBehavior);
		assert(m_danceBehavior);
		SetFlyBehavior(std::move(flyBehavior));
	}

	void Quack() const
	{
		m_quackBehavior->Quack();
	}

	static void Swim()
	{
		std::cout << "I'm swimming" << std::endl;
	}

	void Fly() const
	{
		m_flyBehavior->Fly();
		const auto fliesCount = m_flyBehavior->GetCount();
		if (NeedDoAction(fliesCount))
		{
			Quack();
		}
	}

	void Dance() const
	{
		m_danceBehavior->Dance();
	}

	void SetFlyBehavior(std::unique_ptr<IFlyBehavior>&& flyBehavior)
	{
		assert(flyBehavior);
		m_flyBehavior = std::move(flyBehavior);
	}

	void FlyAction() override
	{
		Quack();
	}

	virtual void Display() const = 0;
	virtual ~Duck() = default;

private:
	std::unique_ptr<IFlyBehavior> m_flyBehavior;
	std::unique_ptr<IQuackBehavior> m_quackBehavior;
	std::unique_ptr<IDanceBehavior> m_danceBehavior;

	static bool NeedDoAction(const int fliesCount)
	{
		return fliesCount % 2 != 0;
	};
};

#endif
