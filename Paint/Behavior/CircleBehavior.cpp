//
// Created by Вадим Патрушев on 01.10.2026.
//

#include "CircleBehavior.h"

#include <sstream>
#include <iomanip>
#include <iostream>

CircleBehavior::CircleBehavior(const Point position, const double radius)
	: m_position(position), m_radius(radius)
{
}

void CircleBehavior::Draw(ICanvas& canvas, const Color color) const
{
	canvas.SetColor(color);
	std::cout << "draw circle behavior" << std::endl;
}

std::string CircleBehavior::GetInfo() const
{
	std::ostringstream output;
	output << std::fixed << std::setprecision(2)
		<< m_position.m_x << " " << m_position.m_y << " " << m_radius;
	return output.str();
}

std::string CircleBehavior::GetName() const
{
	return "circle";
}

void CircleBehavior::Move(const Point position)
{
	m_position.m_x += position.m_x;
	m_position.m_y += position.m_y;
}

std::unique_ptr<IShapeBehavior> CircleBehavior::Clone() const
{
	return std::make_unique<CircleBehavior>(*this);
}
