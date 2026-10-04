//
// Created by Вадим Патрушев on 04.10.2026.
//

#include "AddCommand.h"
#include "Picture.h"
#include "Parser/ParseBehavior.h"
#include <sstream>
#include <string>

AddCommand::AddCommand(std::stringstream& input)
{
	std::string colorStr;
	if (!(input >> m_id >> colorStr >> m_type))
	{
		throw std::invalid_argument("Invalid AddShape syntax");
	}

	std::stringstream colorStream(colorStr.substr(1));
	colorStream >> std::hex >> m_color;

	std::getline(input, m_params);
}

void AddCommand::Execute(Picture& picture, ICanvas& canvas)
{
	if (picture.GetShape(m_id) != nullptr)
	{
		throw std::runtime_error("Shape with id '" + m_id + "' already exists");
	}

	std::stringstream params(m_params);
	auto behavior = ParseBehavior(m_type, params);

	auto shape = std::make_unique<Shape>(m_id, m_color, std::move(behavior));
	picture.AddShape(std::move(shape));
}