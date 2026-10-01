//
// Created by Вадим Патрушев on 28.09.2026.
//

#include "Canvas.h"
#include <iostream>

Canvas::Canvas()
{
	std::cout << "Create canvas" << std::endl;
}

Canvas::~Canvas()
{
	std::cout << "Delete canvas" << std::endl;
}

void Canvas::SetColor(Color color)
{
	std::cout << "Set color in canvas" << std::endl;
}

void Canvas::MoveTo(Point position)
{
	std::cout << "Move to in canvas" << std::endl;
}

void Canvas::LineTo(Point position)
{
	std::cout << "Line to in canvas" << std::endl;
}

void Canvas::DrawEllipse(Point position, Point newPosition)
{
	std::cout << "Draw ellipse in canvas" << std::endl;
}

void Canvas::DrawText(Point position, double fontSize, std::string& text)
{
	std::cout << "Draw text in canvas" << std::endl;
}