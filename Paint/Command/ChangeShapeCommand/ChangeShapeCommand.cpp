//
// Created by Вадим Патрушев on 04.10.2026.
//

#include "ChangeShapeCommand.h"
#include "Parser/ParseBehavior.h"
#include <sstream>

ChangeShapeCommand::ChangeShapeCommand(std::stringstream& input)
{
	if (!(input >> m_id >> m_type))
	{
		throw std::invalid_argument("Invalid shape '" + m_id + "' found");
	}

	std::getline(input, m_params);
}

void ChangeShapeCommand::Execute(Picture& picture, ICanvas&)
{
	auto* shape = picture.GetShape(m_id);
	if (!shape)
	{
		throw std::runtime_error("Shape '" + m_id + "' not found");
	}
	std::stringstream paramsStream(m_params);
	auto behavior = ParseBehavior(m_type, paramsStream);
	shape->ChangeBehavior(std::move(behavior));
}