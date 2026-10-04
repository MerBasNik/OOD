//
// Created by Вадим Патрушев on 04.10.2026.
//

#include "MovePictureCommand.h"
#include <sstream>

MovePictureCommand::MovePictureCommand(std::stringstream& input)
{
	if (!(input >> m_position.m_x >> m_position.m_y))
	{
		throw std::invalid_argument("Неправильный аргумент");
	}
}

void MovePictureCommand::Execute(Picture& picture, ICanvas& canvas)
{
	picture.MovePicture(m_position);
}