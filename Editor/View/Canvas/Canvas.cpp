//
// Created by Вадим Патрушев on 15.10.2026.
//

#include "Canvas.h"
#include <cmath>
#include <numbers>

const size_t ELLIPSE_POINT_COUNT = 100;
const float OUTLINE_THICKNESS = 2.f;

Canvas::Canvas(sf::RenderTarget& target, const sf::Vector2f origin)
	: m_target(target)
	, m_origin(origin)
{
}

void Canvas::DrawRectangle(const Frame& frame, const Color fillColor, const Color borderColor)
{
	sf::RectangleShape rectangle(sf::Vector2f(static_cast<float>(frame.m_width), static_cast<float>(frame.m_height)));
	rectangle.setPosition(ToTargetPoint({ frame.m_left, frame.m_top }));
	SetStyle(rectangle, fillColor, borderColor);
	m_target.draw(rectangle);
}

void Canvas::DrawEllipse(const Frame& frame, const Color fillColor, const Color borderColor)
{
	const Point center = frame.GetCenter();
	const double rx = frame.m_width / 2;
	const double ry = frame.m_height / 2;

	sf::ConvexShape ellipse(ELLIPSE_POINT_COUNT);
	for (size_t i = 0; i < ELLIPSE_POINT_COUNT; ++i)
	{
		const double angle = 2 * std::numbers::pi * static_cast<double>(i) / ELLIPSE_POINT_COUNT;
		ellipse.setPoint(i, ToTargetPoint({ center.m_x + rx * std::cos(angle), center.m_y + ry * std::sin(angle) }));
	}
	SetStyle(ellipse, fillColor, borderColor);
	m_target.draw(ellipse);
}

void Canvas::DrawPolygon(const std::vector<Point>& vertices, const Color fillColor, const Color borderColor)
{
	sf::ConvexShape polygon(vertices.size());
	for (size_t i = 0; i < vertices.size(); ++i)
	{
		polygon.setPoint(i, ToTargetPoint(vertices[i]));
	}
	SetStyle(polygon, fillColor, borderColor);
	m_target.draw(polygon);
}

sf::Color Canvas::ConvertColorToSf(const Color color)
{
	return {
		static_cast<std::uint8_t>((color >> 16) & 0xFF),
		static_cast<std::uint8_t>((color >> 8) & 0xFF),
		static_cast<std::uint8_t>(color & 0xFF),
	};
}

sf::Vector2f Canvas::ToTargetPoint(const Point point) const
{
	return m_origin + sf::Vector2f(static_cast<float>(point.m_x), static_cast<float>(point.m_y));
}

void Canvas::SetStyle(sf::Shape& shape, const Color fillColor, const Color borderColor)
{
	shape.setFillColor(ConvertColorToSf(fillColor));
	shape.setOutlineColor(ConvertColorToSf(borderColor));
	shape.setOutlineThickness(-OUTLINE_THICKNESS);
}
