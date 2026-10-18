//
// Created by Вадим Патрушев on 15.10.2026.
//

#ifndef OOD_EDITOR_RECTANGLE_H
#define OOD_EDITOR_RECTANGLE_H

#include "Shape.h"

class Rectangle : public Shape
{
public:
	Rectangle(const Frame& frame, Color color);
	ShapeType GetType() const override;
	bool HitTest(Point point) const override;
	void Draw(ICanvas& canvas) const override;
	std::unique_ptr<IShape> Clone() const override;
};

#endif // OOD_EDITOR_RECTANGLE_H
