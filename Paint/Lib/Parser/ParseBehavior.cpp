//
// Created by Вадим Патрушев on 04.10.2026.
//

#include "CircleBehavior.h"
#include "IShapeBehavior.h"
#include "LineBehavior.h"
#include "RectangleBehavior.h"
#include "TextBehavior.h"
#include "TriangleBehavior.h"
#include <string>
#include <sstream>

std::unique_ptr<IShapeBehavior> ParseBehavior(const std::string& typeName, std::stringstream& params)
{
	if (typeName == "circle")
	{
		double x;
		double y;
		double radius;
		if (!(params >> x >> y >> radius))
		{
			throw std::invalid_argument("Invalid circle parameters");
		}
		if (radius < 0)
		{
			throw std::invalid_argument("Circle radius must be non-negative");
		}
		return std::make_unique<CircleBehavior>(Point{x, y}, radius);
	}

	if (typeName == "rectangle")
	{
		double left, top, width, height;
		if (!(params >> left >> top >> width >> height))
		{
			throw std::invalid_argument("Invalid rectangle parameters");
		}
		if (width < 0 || height < 0)
		{
			throw std::invalid_argument("Rectangle dimensions must be non-negative");
		}
		return std::make_unique<RectangleBehavior>(Point{left, top}, width, height);
	}

	if (typeName == "triangle")
	{
		double x1, y1, x2, y2, x3, y3;
		if (!(params >> x1 >> y1 >> x2 >> y2 >> x3 >> y3))
		{
			throw std::invalid_argument("Invalid triangle parameters");
		}
		return std::make_unique<TriangleBehavior>(
			Point{x1, y1}, Point{x2, y2}, Point{x3, y3}
		);
	}

	if (typeName == "line")
	{
		double x1, y1, x2, y2;
		if (!(params >> x1 >> y1 >> x2 >> y2))
		{
			throw std::invalid_argument("Invalid line parameters");
		}
		return std::make_unique<LineBehavior>(Point{x1, y1}, Point{x2, y2});
	}

	if (typeName == "text")
	{
		double left, top, fontSize;
		if (!(params >> left >> top >> fontSize))
		{
			throw std::invalid_argument("Invalid text parameters");
		}
		if (fontSize < 0)
		{
			throw std::invalid_argument("Font size must be non-negative");
		}
		std::string text;
		std::getline(params, text);
		if (!text.empty() && text[0] == ' ')
		{
			text = text.substr(1);
		}
		return std::make_unique<TextBehavior>(Point{left, top}, fontSize, text);
	}

	throw std::invalid_argument("Unknown shape type: " + typeName);
}