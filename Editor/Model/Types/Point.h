//
// Created by Вадим Патрушев on 15.10.2026.
//

#ifndef OOD_EDITOR_POINT_H
#define OOD_EDITOR_POINT_H

struct Point
{
	double m_x;
	double m_y;
	bool operator==(const Point& other) const
	{
		return m_x == other.m_x && m_y == other.m_y;
	}

	bool operator!=(const Point& other) const
	{
		return !(*this == other);
	}
};

#endif // OOD_EDITOR_POINT_H
