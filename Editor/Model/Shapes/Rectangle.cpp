//
// Created by Вадим Патрушев on 15.10.2026.
//

#include "Rectangle.h"

Rectangle::Rectangle(const Frame& frame, const Color color)
	: Shape(frame, color)
{
}

ShapeType Rectangle::GetType() const
{
	return ShapeType::Rectangle;
}

bool Rectangle::HitTest(const Point point) const
{
	return GetFrame().IsContains(point);
}

void Rectangle::Draw(ICanvas& canvas) const
{
	canvas.DrawRectangle(GetFrame(), GetColor());
}

std::unique_ptr<IShape> Rectangle::Clone() const
{
	return std::make_unique<Rectangle>(*this);
}
