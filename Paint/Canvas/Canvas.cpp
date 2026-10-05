//
// Created by Вадим Патрушев on 28.09.2026.
//

#include "Canvas.h"
#include <iostream>

Canvas::Canvas(unsigned int width, unsigned int height)
	: m_window(sf::VideoMode({ width, height }), "Paint")
{
	bool isLoad = m_font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf");
	if (!isLoad)
	{
		std::cerr << "Ошибка при загрузке шрифта" << std::endl;
	}
}

void Canvas::SetColor(const Color color)
{
	m_color = sf::Color(color);
}

void Canvas::MoveTo(const Point position)
{
	m_position = sf::Vector2f(static_cast<float>(position.m_x), static_cast<float>(position.m_y));
}

void Canvas::LineTo(const Point position)
{
	const auto end = sf::Vector2f(static_cast<float>(position.m_x), static_cast<float>(position.m_y));

	m_lines.push_back({ m_position, end, m_color });

	m_position = end;
}

void Canvas::DrawEllipse(const Point position, const Point radiuses)
{
	m_ellipses.push_back({
		static_cast<float>(position.m_x),
		static_cast<float>(position.m_y),
		static_cast<float>(radiuses.m_x),
		static_cast<float>(radiuses.m_y),
		m_color,
	});
}

void Canvas::DrawText(const Point position, const double fontSize, const std::string& text)
{
	m_texts.push_back({ static_cast<float>(position.m_x), static_cast<float>(position.m_y),
		static_cast<unsigned>(fontSize), text, m_color });
}

void Canvas::Display()
{
	m_window.clear(sf::Color::White);

	DrawAllLines();
	DrawAllEllipses();
	DrawAllTexts();

	m_window.display();
}

void Canvas::DrawAllLines()
{
	for (const auto& line : m_lines)
	{
		const sf::Vertex vertices[] = {
			sf::Vertex(line.m_a, line.m_color),
			sf::Vertex(line.m_b, line.m_color)
		};

		m_window.draw(vertices, 2, sf::PrimitiveType::Lines);
	}
}

void Canvas::DrawAllEllipses()
{
	for (const auto& ellipse : m_ellipses)
	{
		const float maxRadius = std::max(static_cast<float>(ellipse.m_rx), static_cast<float>(ellipse.m_ry));
		sf::CircleShape circle(maxRadius, COUNT_ELLIPSE_DOTS);

		circle.setOrigin(sf::Vector2f(maxRadius, maxRadius));
		circle.setPosition(sf::Vector2f(ellipse.m_cx, ellipse.m_cy));
		circle.setScale(sf::Vector2f(static_cast<float>(ellipse.m_rx) / maxRadius,
			static_cast<float>(ellipse.m_ry) / maxRadius));
		circle.setFillColor(m_color);
		circle.setOutlineColor(m_color);
		circle.setOutlineThickness(1.0f);

		m_window.draw(circle);
	}
}

void Canvas::DrawAllTexts()
{
	for (const auto& text : m_texts)
	{
		sf::Text sfText(m_font);

		sfText.setString(text.m_text);
		sfText.setCharacterSize(text.m_fontSize);
		sfText.setFillColor(text.m_color);
		sfText.setPosition(sf::Vector2f(text.m_x, text.m_y));

		m_window.draw(sfText);
	}
}

void Canvas::HandleClose()
{
	while (const auto event = m_window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			m_window.close();
		}
	}
}