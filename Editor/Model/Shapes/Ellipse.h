//
// Created by Вадим Патрушев on 15.10.2026.
//

#ifndef OOD_EDITOR_ELLIPSE_H
#define OOD_EDITOR_ELLIPSE_H

#include "Shape.h"

class Ellipse : public Shape
{
public:
	Ellipse(const Frame& frame, Color color);
	bool HitTest(Point point) const override;
	std::unique_ptr<IShape> Clone() const override;
	void Accept(IShapeVisitor& visitor) const override;
};

#endif // OOD_EDITOR_ELLIPSE_H
