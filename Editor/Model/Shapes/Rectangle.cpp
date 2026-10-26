//
// Created by Вадим Патрушев on 15.10.2026.
//

#include "Rectangle.h"

Rectangle::Rectangle(const Frame& frame, const Color color)
	: Shape(frame, color)
{
}

void Rectangle::Accept(IShapeVisitor& visitor) const
{
	visitor.Visit(*this);
}

bool Rectangle::HitTest(const Point point) const
{
	return GetFrame().IsContains(point);
}

std::unique_ptr<IShape> Rectangle::Clone() const
{
	return std::make_unique<Rectangle>(*this);
}
