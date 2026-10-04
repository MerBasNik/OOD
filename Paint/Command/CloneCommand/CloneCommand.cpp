//
// Created by Вадим Патрушев on 04.10.2026.
//

#include "CloneCommand.h"
#include <sstream>

CloneCommand::CloneCommand(std::stringstream& input)
{
	if (!(input >> m_id >> m_newId))
	{
		throw std::invalid_argument("Invalid Clone ID");
	}
}

void CloneCommand::Execute(Picture& picture, ICanvas& canvas)
{
	auto* shape = picture.GetShape(m_id);
	if (!shape)
	{
		throw std::runtime_error("Shape '" + m_id + "' not found");
	}
	auto newShape = shape->Clone();
	newShape->SetId(m_newId);
	picture.AddShape(std::move(newShape));
}