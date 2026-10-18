//
// Created by Вадим Патрушев on 15.10.2026.
//

#include "Ellipse.h"

Ellipse::Ellipse(const Frame& frame, const Color color)
	: Shape(frame, color)
{
}

ShapeType Ellipse::GetType() const
{
	return ShapeType::Ellipse;
}

bool Ellipse::HitTest(const Point point) const
{
	const Frame frame = GetFrame();
	const double rx = frame.m_width / 2;
	const double ry = frame.m_height / 2;
	if (rx <= 0 || ry <= 0)
	{
		return false;
	}
	const Point center = frame.GetCenter();
	const double dx = (point.m_x - center.m_x) / rx;
	const double dy = (point.m_y - center.m_y) / ry;
	return dx * dx + dy * dy <= 1.0;
}

void Ellipse::Draw(ICanvas& canvas) const
{
	canvas.DrawEllipse(GetFrame(), GetColor());
}

std::unique_ptr<IShape> Ellipse::Clone() const
{
	return std::make_unique<Ellipse>(*this);
}
