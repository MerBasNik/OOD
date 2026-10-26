//
// Created by Вадим Патрушев on 26.10.2026.
//

#include "ShapeSerializer.h"
#include "Model/Shapes/Ellipse.h"
#include "Model/Shapes/Rectangle.h"
#include "Model/Shapes/Triangle.h"
#include <iomanip>

const int SET_PRECISION = 17;

ShapeSerializer::ShapeSerializer(std::ostream& output)
	: m_output(output)
{
}

void ShapeSerializer::Visit(const Rectangle& rectangle)
{
	Write(RECTANGLE_NAME, rectangle);
}

void ShapeSerializer::Visit(const Ellipse& ellipse)
{
	Write(ELLIPSE_NAME, ellipse);
}

void ShapeSerializer::Visit(const Triangle& triangle)
{
	Write(TRIANGLE_NAME, triangle);
}

void ShapeSerializer::Write(const std::string& typeName, const IShape& shape)
{
	const Frame frame = shape.GetFrame();
	m_output << typeName << " "
			 << std::setprecision(SET_PRECISION) << frame.m_left << " " << frame.m_top << " "
			 << frame.m_width << " " << frame.m_height << " "
			 << std::hex << std::setw(6) << std::setfill('0') << shape.GetColor()
			 << std::dec << std::setfill(' ') << "\n";
}
