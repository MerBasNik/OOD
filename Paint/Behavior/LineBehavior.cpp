//
// Created by Вадим Патрушев on 01.10.2026.
//

#include "LineBehavior.h"
#include <iomanip>
#include <sstream>
#include <iostream>

LineBehavior::LineBehavior(const Point start, const Point end)
	: m_start(start), m_end(end)
{
}

void LineBehavior::Draw(ICanvas& canvas, Color color) const
{
	canvas.SetColor(color);
	canvas.MoveTo(Point{ m_start.m_x, m_start.m_y });
	canvas.LineTo(Point{ m_end.m_x, m_end.m_y });
}

std::string LineBehavior::GetName() const
{
	return "line";
}

std::string LineBehavior::GetInfo() const
{
	std::ostringstream output;
	output << m_start.m_x << " " << m_start.m_y << " " << m_end.m_x << " " << m_end.m_y;
	return output.str();
}

void LineBehavior::Move(const Point position)
{
	m_start.m_x += position.m_x;
	m_start.m_y += position.m_y;
	m_end.m_x += position.m_x;
	m_end.m_y += position.m_y;
}

std::unique_ptr<IShapeBehavior> LineBehavior::Clone() const
{
	return std::make_unique<LineBehavior>(*this);
};
