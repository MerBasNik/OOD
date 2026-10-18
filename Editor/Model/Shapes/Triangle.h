//
// Created by Вадим Патрушев on 15.10.2026.
//

#ifndef OOD_EDITOR_TRIANGLE_H
#define OOD_EDITOR_TRIANGLE_H

#include "Shape.h"

class Triangle : public Shape
{
public:
	Triangle(const Frame& frame, Color color);
	ShapeType GetType() const override;
	bool HitTest(Point point) const override;
	void Draw(ICanvas& canvas) const override;
	std::unique_ptr<IShape> Clone() const override;
	std::vector<Point> GetVertices() const;

private:
	static double Cross(Point a, Point b, Point c);
};

#endif // OOD_EDITOR_TRIANGLE_H
