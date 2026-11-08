//
// Created by Вадим Патрушев on 15.10.2026.
//

#include "Shape.h"

Shape::Shape(const Frame& frame, const Color fillColor, const Color borderColor)
	: m_frame(frame)
	, m_fillColor(fillColor)
	, m_borderColor(borderColor)
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

Color Shape::GetFillColor() const
{
	return m_fillColor;
}

Color Shape::GetBorderColor() const
{
	return m_borderColor;
}

void Shape::SetFillColor(const Color color)
{
	m_fillColor = color;
}

void Shape::SetBorderColor(const Color color)
{
	m_borderColor = color;
}

TimeInterval Shape::GetTimeInterval() const
{
	return m_timeInterval;
}

void Shape::SetTimeInterval(const TimeInterval timeInterval)
{
	m_timeInterval = timeInterval;
}