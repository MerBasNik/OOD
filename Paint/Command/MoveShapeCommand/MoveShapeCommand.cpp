//
// Created by Вадим Патрушев on 04.10.2026.
//

#include "MoveShapeCommand.h"
#include "Picture.h"
#include <sstream>
#include <string>

MoveShapeCommand::MoveShapeCommand(std::stringstream& input)
{
	if (!(input >> m_id >> m_dx >> m_dy))
	{
		throw std::invalid_argument("Invalid MoveShape syntax");
	}
}

void MoveShapeCommand::Execute(Picture& picture, ICanvas&)
{
	auto* shape = picture.GetShape(m_id);
	if (!shape)
	{
		throw std::runtime_error("Shape '" + m_id + "' not found");
	}
	shape->Move(Point{m_dx, m_dy});
}