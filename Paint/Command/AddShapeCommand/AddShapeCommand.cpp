//
// Created by Вадим Патрушев on 04.10.2026.
//

#include "AddShapeCommand.h"
#include "Parser/ParseBehavior.h"
#include "Picture.h"
#include <sstream>
#include <string>

AddShapeCommand::AddShapeCommand(std::stringstream& input)
{
	std::string colorStr;
	if (!(input >> m_id >> colorStr >> m_type))
	{
		throw std::invalid_argument("Неправильный аргумент");
	}

	std::stringstream colorStream(colorStr.substr(1));
	colorStream >> std::hex >> m_color;

	std::getline(input, m_params);
}

void AddShapeCommand::Execute(Picture& picture, ICanvas& canvas)
{
	if (picture.GetShape(m_id) != nullptr)
	{
		throw std::runtime_error("Фигура с id: " + m_id + " уже есть");
	}

	std::stringstream params(m_params);
	auto behavior = ParseBehavior(m_type, params);

	auto shape = std::make_unique<Shape>(m_id, m_color, std::move(behavior));
	picture.AddShape(std::move(shape));
}
