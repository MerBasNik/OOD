//
// Created by Вадим Патрушев on 15.10.2026.
//

#ifndef OOD_EDITOR_TRIANGLE_H
#define OOD_EDITOR_TRIANGLE_H

#include "Shape.h"
#include <vector>

class Triangle : public Shape
{
public:
	Triangle(const Frame& frame, Color color);
	bool HitTest(Point point) const override;
	std::unique_ptr<IShape> Clone() const override;
	std::vector<Point> GetVertices() const;
	void Accept(IShapeVisitor& visitor) const override;

private:
	static double Cross(Point a, Point b, Point c);
};

#endif // OOD_EDITOR_TRIANGLE_H
