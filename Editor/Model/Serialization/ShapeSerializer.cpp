//
// Created by Вадим Патрушев on 26.10.2026.
//

#include "ShapeSerializer.h"
#include "Model/Shapes/Ellipse.h"
#include "Model/Shapes/GroupShape.h"
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

void ShapeSerializer::Visit(const GroupShape& group)
{
	m_output << GROUP_NAME << "\n";
	for (size_t i = 0; i < group.GetChildCount(); ++i)
	{
		group.GetChild(i).Accept(*this);
	}
	m_output << GROUP_END_NAME << "\n";
}

void ShapeSerializer::Write(const std::string& typeName, const IShape& shape)
{
	const Frame frame = shape.GetFrame();
	const TimeInterval timeInterval = shape.GetTimeInterval();
	m_output << typeName << " "
			 << std::setprecision(SET_PRECISION) << frame.m_left << " " << frame.m_top << " "
			 << frame.m_width << " " << frame.m_height << " "
			 << std::hex << std::setfill('0') << std::setw(6) << shape.GetFillColor() << " "
			 << std::setw(6) << shape.GetBorderColor()
			 << std::dec << std::setfill(' ') << " "
			 << timeInterval.m_start << " " << timeInterval.m_end << "\n";
}
