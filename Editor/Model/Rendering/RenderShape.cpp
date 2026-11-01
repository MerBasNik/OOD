//
// Created by Вадим Патрушев on 26.10.2026.
//

#include "RenderShape.h"
#include "Model/Shapes/Ellipse.h"
#include "Model/Shapes/Rectangle.h"
#include "Model/Shapes/Triangle.h"

RenderShape::RenderShape(ICanvas& canvas)
	: m_canvas(canvas)
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
