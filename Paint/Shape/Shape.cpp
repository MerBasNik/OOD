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

std::string Shape::GetId() const
{
	return m_id;
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
}

void Shape::Move(const Point position)
{
	m_behavior->Move(position);
}

void Shape::Draw(ICanvas& canvas) const
{
	m_behavior->Draw(canvas, m_color);
}

void Shape::ChangeBehavior(std::unique_ptr<IShapeBehavior> newBehavior)
{
	m_behavior = std::move(newBehavior);
}

std::unique_ptr<IShape> Shape::Clone() const
{
	return std::make_unique<Shape>(m_id, m_color, std::move(m_behavior));
}