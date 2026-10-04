//
// Created by Вадим Патрушев on 04.10.2026.
//

#include "AddShapeCommand.h"
#include "Picture.h"
#include "Parser/ParseBehavior.h"
#include <sstream>
#include <string>

AddShapeCommand::AddShapeCommand(std::stringstream& input)
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

void AddShapeCommand::Execute(Picture& picture, ICanvas& canvas)
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

// AddShape sh1 #ff00ff circle 100 110 15
// AddShape sh2 #febb38 circle 100 200 25
// AddShape sh3 #123456 rectangle 10 20 30 40
// AddShape sh4 #00fefe triangle 0 0 10 0 0 10
// AddShape sh5 #fefefe line 10 20 35 -88
// AddShape sh6 #ffaa88 text 100.3 100.2 12.8 Hello world