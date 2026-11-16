#ifndef OOD_CANVASZOOMCONTROLS_H
#define OOD_CANVASZOOMCONTROLS_H

#include "CanvasView.h"
#include <SFML/Graphics.hpp>
#include <optional>
#include <string>

class CanvasZoomControls
{
public:
	static constexpr float WIDTH = 160;
	static constexpr float HEIGHT = 32;

	CanvasZoomControls(CanvasView& canvas, const sf::Font& font, sf::Vector2f position);
	void Draw(sf::RenderTarget& target) const;
	bool HandleClick(sf::Vector2f point);
	bool IsContains(sf::Vector2f point) const;
	bool SetHoverPoint(std::optional<sf::Vector2f> point);

private:
	CanvasView& m_canvasView;
	const sf::Font& m_font;
	sf::Vector2f m_position;
	std::optional<sf::Vector2f> m_hoverPoint;

	sf::FloatRect GetButtonRect(size_t index) const;
	void DrawButton(sf::RenderTarget& target, size_t index, const std::string& label) const;
};

#endif
