//
// Created by Вадим Патрушев on 15.10.2026.
//

#include "Shape.h"

Shape::Shape(const Frame& frame, const Color color)
	: m_frame(frame)
	, m_color(color)
{
}

Frame Shape::GetFrame() const
{
	return m_frame;
}

void Shape::SetFrame(const Frame& frame)
{
	m_frame = frame;
}

Color Shape::GetColor() const
{
	return m_color;
}
