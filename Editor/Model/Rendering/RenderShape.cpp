//
// Created by Вадим Патрушев on 26.10.2026.
//

#include "RenderShape.h"
#include "Model/Shapes/Ellipse.h"
#include "Model/Shapes/GroupShape.h"
#include "Model/Shapes/Rectangle.h"
#include "Model/Shapes/Triangle.h"

RenderShape::RenderShape(ICanvas& canvas, const std::optional<double> time)
	: m_canvas(canvas)
	, m_time(time)
{
}

void RenderShape::Visit(const Rectangle& rectangle)
{
	m_canvas.DrawRectangle(rectangle.GetFrame(), rectangle.GetFillColor(), rectangle.GetBorderColor());
}

void RenderShape::Visit(const Ellipse& ellipse)
{
	m_canvas.DrawEllipse(ellipse.GetFrame(), ellipse.GetFillColor(), ellipse.GetBorderColor());
}

void RenderShape::Visit(const Triangle& triangle)
{
	m_canvas.DrawPolygon(triangle.GetVertices(), triangle.GetFillColor(), triangle.GetBorderColor());
}

void RenderShape::Visit(const GroupShape& group)
{
	for (size_t i = 0; i < group.GetChildCount(); ++i)
	{
		const IShape& child = group.GetChild(i);
		if (!m_time || child.GetTimeInterval().IsContains(*m_time))
		{
			child.Accept(*this);
		}
	}
}
