#include "CanvasZoomControls.h"
#include <cmath>

CanvasZoomControls::CanvasZoomControls(CanvasView& canvas, const sf::Font& font, const sf::Vector2f position)
	: m_canvasView(canvas)
	, m_font(font)
	, m_position(position)
{
}

void CanvasZoomControls::Draw(sf::RenderTarget& target) const
{
	DrawButton(target, 0, "-");
	DrawButton(target, 1, std::to_string(std::lround(m_canvasView.GetZoom() * 100)) + "%");
	DrawButton(target, 2, "+");
}

void CanvasZoomControls::DrawButton(sf::RenderTarget& target, const size_t index, const std::string& label) const
{
	const sf::FloatRect rect = GetButtonRect(index);
	const bool hovered = m_hoverPoint && rect.contains(*m_hoverPoint);
	sf::RectangleShape background(rect.size);
	background.setPosition(rect.position);
	background.setFillColor(hovered ? sf::Color(228, 236, 248) : sf::Color(250, 250, 250));
	background.setOutlineColor(sf::Color(170, 170, 170));
	background.setOutlineThickness(-1);
	target.draw(background);
	sf::Text text(m_font, label, 14);
	text.setFillColor(sf::Color(30, 30, 30));
	const sf::FloatRect bounds = text.getLocalBounds();
	text.setOrigin(bounds.position + bounds.size / 2.f);
	text.setPosition(rect.position + rect.size / 2.f);
	target.draw(text);
}

bool CanvasZoomControls::HandleClick(const sf::Vector2f point)
{
	if (!IsContains(point))
	{
		return false;
	}
	if (GetButtonRect(0).contains(point))
	{
		m_canvasView.ZoomOut();
	}
	else if (GetButtonRect(1).contains(point))
	{
		m_canvasView.ResetZoom();
	}
	else if (GetButtonRect(2).contains(point))
	{
		m_canvasView.ZoomIn();
	}
	return true;
}

bool CanvasZoomControls::IsContains(const sf::Vector2f point) const
{
	return sf::FloatRect(m_position, { WIDTH, HEIGHT }).contains(point);
}

bool CanvasZoomControls::SetHoverPoint(const std::optional<sf::Vector2f> point)
{
	const auto previous = m_hoverPoint;
	m_hoverPoint = point && IsContains(*point) ? point : std::nullopt;
	return previous != m_hoverPoint;
}

sf::FloatRect CanvasZoomControls::GetButtonRect(const size_t index) const
{
	const float left = index == 0 ? 0 : index == 1 ? 40 : 128;
	const float width = index == 1 ? 80 : 32;
	return { m_position + sf::Vector2f(left, 0), { width, HEIGHT } };
}
