//
// Created by Вадим Патрушев on 01.10.2026.
//

#include "RectangleBehavior.h"
#include <sstream>
#include <iomanip>
#include <iostream>

RectangleBehavior::RectangleBehavior(const Point position, const double width, const double height)
	: m_position(position), m_width(width), m_height(height)
{
}

void RectangleBehavior::Draw(ICanvas& canvas, const Color color) const
{
	canvas.SetColor(color);
	std::cout << "draw rectangle behavior" << std::endl;
}

std::string RectangleBehavior::GetName() const
{
	return "rectangle";
}

std::string RectangleBehavior::GetInfo() const
{
	std::ostringstream output;
	output << m_position.m_x << " " << m_position.m_y;
	return output.str();
}

void RectangleBehavior::Move(const Point position)
{
	m_position.m_x += position.m_x;
	m_position.m_y += position.m_y;
}

std::unique_ptr<IShapeBehavior> RectangleBehavior::Clone() const
{
	return std::make_unique<RectangleBehavior>(*this);
}