//
// Created by Вадим Патрушев on 15.10.2026.
//

#include "Triangle.h"

double Triangle::Cross(const Point a, const Point b, const Point c)
{
	return (b.m_x - a.m_x) * (c.m_y - a.m_y) - (b.m_y - a.m_y) * (c.m_x - a.m_x);
}

Triangle::Triangle(const Frame& frame, const Color color)
	: Shape(frame, color)
{
}

void Triangle::Accept(IShapeVisitor& visitor) const
{
	visitor.Visit(*this);
}

std::vector<Point> Triangle::GetVertices() const
{
	const Frame frame = GetFrame();
	return {
		{ frame.m_left + frame.m_width / 2, frame.m_top },
		{ frame.GetRight(), frame.GetBottom() },
		{ frame.m_left, frame.GetBottom() },
	};
}

bool Triangle::HitTest(const Point point) const
{
	const auto vertices = GetVertices();
	const double d1 = Cross(vertices[0], vertices[1], point);
	const double d2 = Cross(vertices[1], vertices[2], point);
	const double d3 = Cross(vertices[2], vertices[0], point);

	const bool hasNegative = d1 < 0 || d2 < 0 || d3 < 0;
	const bool hasPositive = d1 > 0 || d2 > 0 || d3 > 0;
	return !(hasNegative && hasPositive);
}

std::unique_ptr<IShape> Triangle::Clone() const
{
	return std::make_unique<Triangle>(*this);
}
