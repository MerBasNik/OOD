//
// Created by Вадим Патрушев on 13.09.2026.
//

#ifndef OOD_DUCK_H
#define OOD_DUCK_H

#include "../behavior/DanceBehavior.h"
#include "../behavior/DisplayBehavior.h"
#include "../behavior/FlyBehavior.h"
#include "../behavior/QuackBehavior.h"

#include <cassert>
#include <iostream>

class Duck
{
public:
	Duck(FlyBehavior fly,
		DanceBehavior dance,
		QuackBehavior quack,
		DisplayBehavior display)
		: m_fly(std::move(fly))
		, m_dance(std::move(dance))
		, m_quack(std::move(quack))
		, m_display(std::move(display))
	{
		assert(m_fly);
		assert(m_quack);
		assert(m_dance);
		assert(m_display);
	}

	void SetFlyBehavior(FlyBehavior fly)
	{
		assert(fly);
		m_fly = std::move(fly);
	}

	void SetDanceBehavior(DanceBehavior dance)
	{
		assert(dance);
		m_dance = std::move(dance);
	}

	void SetQuackBehavior(QuackBehavior quack)
	{
		assert(quack);
		m_quack = std::move(quack);
	}

	void SetDisplayBehavior(DisplayBehavior display)
	{
		assert(display);
		m_display = std::move(display);
	}

	void Fly()
	{
		m_fliesCount = m_fly();
		if (NeedDoAction(m_fliesCount))
		{
			m_quack();
		}
		std::cout << "Fly number: " << m_fliesCount << std::endl;
	}

	void Quack()
	{
		m_quack();
	}

	void Dance()
	{
		m_dance();
	}

	void Display()
	{
		m_display();
	}

	static void Swim()
	{
		std::cout << "I'm swimming" << std::endl;
	}

private:
	FlyBehavior m_fly;
	DanceBehavior m_dance;
	QuackBehavior m_quack;
	DisplayBehavior m_display;

	int m_fliesCount = 0;

	static bool NeedDoAction(const int fliesCount)
	{
		return fliesCount % 2 == 0;
	};
};

#endif // OOD_DUCK_H
