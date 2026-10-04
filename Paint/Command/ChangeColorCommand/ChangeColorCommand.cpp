//
// Created by Вадим Патрушев on 04.10.2026.
//

#include "ChangeColorCommand.h"
#include <sstream>

ChangeColorCommand::ChangeColorCommand(std::stringstream& input)
{
	std::string colorStr;
	if (!(input >> m_id >> colorStr))
	{
		throw std::invalid_argument("Invalid input");
	}

	std::stringstream colorStream(colorStr.substr(1));
	colorStream >> std::hex >> m_color;
}

void ChangeColorCommand::Execute(Picture& picture, ICanvas& canvas)
{
	auto* shape = picture.GetShape(m_id);
	if (!shape)
	{
		throw std::runtime_error("Shape '" + m_id + "' not found");
	}

	shape->ChangeColor(m_color);
}