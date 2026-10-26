//
// Created by Вадим Патрушев on 26.10.2026.
//

#ifndef OOD_EDITOR_ISHAPEVISITOR_H
#define OOD_EDITOR_ISHAPEVISITOR_H

class Rectangle;
class Ellipse;
class Triangle;

class IShapeVisitor
{
public:
	virtual ~IShapeVisitor() = default;
	virtual void Visit(const Rectangle& rectangle) = 0;
	virtual void Visit(const Ellipse& ellipse) = 0;
	virtual void Visit(const Triangle& triangle) = 0;
};

#endif // OOD_EDITOR_ISHAPEVISITOR_H
