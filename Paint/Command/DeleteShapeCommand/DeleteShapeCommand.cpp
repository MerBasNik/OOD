//
// Created by Вадим Патрушев on 04.10.2026.
//

#include "DeleteShapeCommand.h"
#include <sstream>

DeleteShapeCommand::DeleteShapeCommand(std::stringstream& input)
{
	if (!(input >> m_id))
	{
		throw std::invalid_argument("Invalid input");
	}
}

void DeleteShapeCommand::Execute(Picture& picture, ICanvas& canvas)
{
	if (!picture.GetShape(m_id))
	{
		throw std::runtime_error("Shape '" + m_id + "' not found");
	}
	picture.DeleteShape(m_id);
}