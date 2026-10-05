//
// Created by Вадим Патрушев on 04.10.2026.
//

#include "CircleBehavior.h"
#include "IShapeBehavior.h"
#include "LineBehavior.h"
#include "RectangleBehavior.h"
#include "TextBehavior.h"
#include "TriangleBehavior.h"
#include <sstream>
#include <string>

std::unique_ptr<IShapeBehavior> ParseBehavior(const std::string& typeName, std::stringstream& params)
{
	if (typeName == "circle")
	{
		Point pos{};
		double radius;
		if (!(params >> pos.m_x >> pos.m_y >> radius))
		{
			throw std::invalid_argument("Неправильный аргумент");
		}
		if (radius < 0)
		{
			throw std::invalid_argument("Радиус должен быть положительным");
		}
		return std::make_unique<CircleBehavior>(pos, radius);
	}

	if (typeName == "rectangle")
	{
		Point pos{};
		double width;
		double height;
		if (!(params >> pos.m_x >> pos.m_y >> width >> height))
		{
			throw std::invalid_argument("Неправильный аргумент");
		}
		if (width < 0 || height < 0)
		{
			throw std::invalid_argument("Ширина и высота должны быть положительными");
		}
		return std::make_unique<RectangleBehavior>(pos, width, height);
	}

	if (typeName == "triangle")
	{
		Point vert1{};
		Point vert2{};
		Point vert3{};
		if (!(params >> vert1.m_x >> vert1.m_y >> vert2.m_x >> vert2.m_y >> vert3.m_x >> vert3.m_y))
		{
			throw std::invalid_argument("Неправильный аргумент");
		}
		return std::make_unique<TriangleBehavior>(vert1, vert2, vert3);
	}

	if (typeName == "line")
	{
		Point point1{};
		Point point2{};
		if (!(params >> point1.m_x >> point1.m_y >> point2.m_x >> point2.m_y))
		{
			throw std::invalid_argument("Неправильный аргумент");
		}
		return std::make_unique<LineBehavior>(point1, point2);
	}

	if (typeName == "text")
	{
		Point pos{};
		double fontSize;
		if (!(params >> pos.m_x >> pos.m_y >> fontSize))
		{
			throw std::invalid_argument("Неправильный аргумент");
		}
		if (fontSize < 0)
		{
			throw std::invalid_argument("fontSize must должен быть положительным");
		}

		std::string text;
		std::getline(params, text);
		return std::make_unique<TextBehavior>(pos, fontSize, text);
	}

	throw std::invalid_argument("Неизвестная фигура: " + typeName);
}