//
// Created by Вадим Патрушев on 04.10.2026.
//

#include "ChangeColor.h"
#include <sstream>

ChangeColor::ChangeColor(std::stringstream& input)
{
	if (!(input >> m_id >> std::hex >> m_color >> std::dec))
	{
		throw std::invalid_argument("Invalid input");
	}
}

void ChangeColor::Execute(Picture& picture, ICanvas& canvas)
{
	auto* shape = picture.GetShape(m_id);
	if (!shape)
	{
		throw std::runtime_error("Shape '" + m_id + "' not found");
	}

	shape->ChangeColor(m_color);
}