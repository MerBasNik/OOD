//
// Created by Вадим Патрушев on 28.09.2026.
//

#ifndef OOD_CANVAS_H
#define OOD_CANVAS_H

#include "EllipseData.h"
#include "ICanvas.h"
#include "LineData.h"
#include "TextData.h"
#include <SFML/Graphics.hpp>

const int COUNT_ELLIPSE_DOTS = 1000;

class Canvas : public ICanvas
{
public:
	Canvas(unsigned int width, unsigned int height);
	~Canvas() override = default;
	void SetColor(Color color) override;
	void MoveTo(Point position) override;
	void LineTo(Point position) override;
	void DrawEllipse(Point position, Point radiuses) override;
	void DrawText(Point position, double fontSize, const std::string& text) override;
	void Display();
	void HandleEvents();

private:
	sf::RenderWindow m_window;
	sf::Color m_color;
	sf::Vector2f m_position;
	sf::Font m_font;
	std::vector<LineData> m_lines;
	std::vector<EllipseData> m_ellipses;
	std::vector<TextData> m_texts;

	void DrawAllLines();
	void DrawAllEllipses();
	void DrawAllTexts();
};

#endif // OOD_CANVAS_H
