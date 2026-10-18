//
// Created by Вадим Патрушев on 18.10.2026.
//

#include "ShapeDefaults.h"
#include <algorithm>

Frame ShapeDefaults::GetDefaultFrame(const Frame& bounds)
{
	const double width = std::min(WIDTH, bounds.m_width);
	const double height = std::min(HEIGHT, bounds.m_height);
	const Point center = bounds.GetCenter();
	return { center.m_x - width / 2, center.m_y - height / 2, width, height };
}
