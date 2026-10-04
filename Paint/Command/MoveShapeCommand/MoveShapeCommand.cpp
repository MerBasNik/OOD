//
// Created by Вадим Патрушев on 04.10.2026.
//

#include "MoveShapeCommand.h"
#include "Picture.h"
#include <sstream>
#include <string>

MoveShapeCommand::MoveShapeCommand(std::stringstream& input)
{
	if (!(input >> m_id >> m_position.m_x >> m_position.m_y))
	{
		throw std::invalid_argument("Неправильный аргумент");
	}
}

void MoveShapeCommand::Execute(Picture& picture, ICanvas&)
{
	auto* shape = picture.GetShape(m_id);
	if (!shape)
	{
		throw std::runtime_error("Фигура с id: " + m_id + " не найдена");
	}
	shape->Move(m_position);
}