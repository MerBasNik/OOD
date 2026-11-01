//
// Created by Вадим Патрушев on 15.10.2026.
//

#ifndef OOD_EDITOR_CANVAS_H
#define OOD_EDITOR_CANVAS_H

#include "ICanvas.h"
#include <SFML/Graphics.hpp>

class Canvas : public ICanvas
{
public:
	Canvas(sf::RenderTarget& target, sf::Vector2f origin);
	void DrawRectangle(const Frame& frame, Color fillColor, Color borderColor) override;
	void DrawEllipse(const Frame& frame, Color fillColor, Color borderColor) override;
	void DrawPolygon(const std::vector<Point>& vertices, Color fillColor, Color borderColor) override;
	static sf::Color ConvertColorToSf(Color color);

private:
	sf::RenderTarget& m_target;
	sf::Vector2f m_origin;

	sf::Vector2f ToTargetPoint(Point point) const;
	static void SetStyle(sf::Shape& shape, Color fillColor, Color borderColor);
};

#endif // OOD_EDITOR_CANVAS_H
