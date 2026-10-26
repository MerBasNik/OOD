//
// Created by Вадим Патрушев on 26.10.2026.
//

#ifndef OOD_EDITOR_SHAPETYPEVISITOR_H
#define OOD_EDITOR_SHAPETYPEVISITOR_H

#include "Model/Shapes/IShape.h"
#include "Model/Shapes/ShapeType.h"
#include <optional>

class ShapeTypeVisitor : public IShapeVisitor
{
public:
	std::optional<ShapeType> m_type;

	void Visit(const Rectangle&) override
	{
		m_type = ShapeType::Rectangle;
	}

	void Visit(const Ellipse&) override
	{
		m_type = ShapeType::Ellipse;
	}

	void Visit(const Triangle&) override
	{
		m_type = ShapeType::Triangle;
	}
};

inline std::optional<ShapeType> GetShapeType(const IShape& shape)
{
	ShapeTypeVisitor visitor;
	shape.Accept(visitor);
	return visitor.m_type;
}

#endif // OOD_EDITOR_SHAPETYPEVISITOR_H
