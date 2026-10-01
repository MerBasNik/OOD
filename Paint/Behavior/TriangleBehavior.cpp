//
// Created by Вадим Патрушев on 01.10.2026.
//

#include "TriangleBehavior.h"
#include <sstream>
#include <iomanip>
#include <iostream>

TriangleBehavior::TriangleBehavior(Point v1, Point v2, Point v3)
	: m_v1(v1), m_v2(v2), m_v3(v3)
{
}

void TriangleBehavior::Draw(ICanvas& canvas, const Color color) const
{
	canvas.SetColor(color);
	std::cout << "draw triangle behavior" << std::endl;
}

std::string TriangleBehavior::GetName() const
{
	return "triangle";
}

std::string TriangleBehavior::GetInfo() const
{
	std::ostringstream output;
	output << std::fixed << std::setprecision(2)
		<< m_v1.m_x << " " << m_v1.m_y << " " << m_v2.m_x << " " << m_v2.m_y << " " << m_v3.m_x << " " << m_v3.m_y;
	return output.str();
}

void TriangleBehavior::Move(const Point position)
{
	MoveVertex(m_v1, position);
	MoveVertex(m_v2, position);
	MoveVertex(m_v3, position);
}

void TriangleBehavior::MoveVertex(Point& vertex, const Point position)
{
	vertex.m_x += position.m_x;
	vertex.m_y += position.m_y;
}