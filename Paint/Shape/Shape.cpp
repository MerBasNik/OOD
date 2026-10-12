//
// Created by Вадим Патрушев on 28.09.2026.
//

#include "Shape.h"

Shape::Shape(
	const std::string& id,
	const Color color,
	std::unique_ptr<IShapeBehavior> behavior)
	: m_id(id)
	, m_color(color)
	, m_behavior(std::move(behavior))
{
}

std::string Shape::GetId() const
{
	return m_id;
}

void Shape::SetId(const std::string& newId)
{
	m_id = newId;
	NotifyObservers();
}

Color Shape::GetColor() const
{
	return m_color;
}

std::string Shape::GetName() const
{
	return m_behavior->GetName();
}

std::string Shape::GetInfo() const
{
	return m_behavior->GetInfo();
}

void Shape::ChangeColor(const Color color)
{
	m_color = color;
	NotifyObservers();
}

void Shape::Move(const Point position)
{
	m_behavior->Move(position);
	NotifyObservers();
}

void Shape::Draw(ICanvas& canvas) const
{
	m_behavior->Draw(canvas, m_color);
	canvas.Display();
}

void Shape::ChangeBehavior(std::unique_ptr<IShapeBehavior> newBehavior)
{
	m_behavior = std::move(newBehavior);
	NotifyObservers();
}

std::unique_ptr<IShape> Shape::Clone() const
{
	auto clonedBehavior = m_behavior ? m_behavior->Clone() : nullptr;
	return std::make_unique<Shape>(m_id, m_color, std::move(clonedBehavior));
}

void Shape::AddObserver(IShapeObserver& observer)
{
	for (const auto ob : m_observers)
	{
		if (ob == &observer)
		{
			return;
		}
	}
	m_observers.push_back(&observer);
}

void Shape::DeleteObserver(IShapeObserver& observer)
{
	for (const auto ob : m_unsubscribers)
	{
		if (ob == &observer)
		{
			return;
		}
	}
	std::erase(m_observers, &observer);
	observer.ResetCountCalls();
	m_unsubscribers.push_back(&observer);
}

void Shape::NotifyObservers()
{
	auto observers = m_observers;
	for (auto* ob : observers)
	{
		if (!IsFindUnsubscribeObserver(*ob))
		{
			ob->Update();
		}
	}
	m_unsubscribers.clear();
}

bool Shape::IsFindUnsubscribeObserver(const IShapeObserver& observer)
{
	for (const auto ob : m_unsubscribers)
	{
		if (ob == &observer)
		{
			return true;
		}
	}
	return false;
}