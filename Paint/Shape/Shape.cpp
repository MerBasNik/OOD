//
// Created by Вадим Патрушев on 28.09.2026.
//

#include "Shape.h"

Shape::Shape(
	const std::string& id,
	const Color color,
	std::unique_ptr<IShapeBehavior> behavior)
	: m_id(id),
	m_color(color),
	m_behavior(std::move(behavior))
{
}

Shape::~Shape()
{
	delete m_behavior.get();
}

std::string Shape::GetId()
{
	return m_id;
}

Color Shape::GetColor()
{
	return m_color;
}

void Shape::ChangeColor(const Color color)
{
	m_color = color;
}

void Shape::Move(Point position)
{

}

void Shape::Draw(ICanvas& canvas)
{

}

void Shape::ChangeBehavior(std::unique_ptr<IShapeBehavior> newBehavior)
{
	m_behavior = std::move(newBehavior);
}