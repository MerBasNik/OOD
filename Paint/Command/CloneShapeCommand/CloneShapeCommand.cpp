//
// Created by Вадим Патрушев on 04.10.2026.
//

#include "CloneShapeCommand.h"
#include <sstream>

CloneShapeCommand::CloneShapeCommand(std::stringstream& input)
{
	if (!(input >> m_id >> m_newId))
	{
		throw std::invalid_argument("Неправильный аргумент");
	}
}

void CloneShapeCommand::Execute(Picture& picture, ICanvas& canvas)
{
	auto* shape = picture.GetShape(m_id);
	if (!shape)
	{
		throw std::runtime_error("Фигура с id: " + m_id + " не найдена");
	}
	auto newShape = shape->Clone();
	newShape->SetId(m_newId);
	picture.AddShape(std::move(newShape));
}