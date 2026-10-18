//
// Created by Вадим Патрушев on 15.10.2026.
//

#ifndef OOD_EDITOR_FRAME_H
#define OOD_EDITOR_FRAME_H

#include "Point.h"

struct Frame
{
	double m_left;
	double m_top;
	double m_width;
	double m_height;

	double GetRight() const
	{
		return m_left + m_width;
	}

	double GetBottom() const
	{
		return m_top + m_height;
	}

	Point GetCenter() const
	{
		return { m_left + m_width / 2, m_top + m_height / 2 };
	}

	bool IsContains(const Point point) const
	{
		return point.m_x >= m_left && point.m_x <= GetRight()
			&& point.m_y >= m_top && point.m_y <= GetBottom();
	}

	bool IsContainsFrame(const Frame& other, const double tolerance = 0) const
	{
		return other.m_left >= m_left - tolerance && other.GetRight() <= GetRight() + tolerance
			&& other.m_top >= m_top - tolerance && other.GetBottom() <= GetBottom() + tolerance;
	}

	bool operator==(const Frame& other) const
	{
		return m_left == other.m_left && m_top == other.m_top
			&& m_width == other.m_width && m_height == other.m_height;
	}
};

#endif // OOD_EDITOR_FRAME_H
