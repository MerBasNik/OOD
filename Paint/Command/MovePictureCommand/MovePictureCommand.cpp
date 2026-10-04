//
// Created by Вадим Патрушев on 04.10.2026.
//

#include "MovePictureCommand.h"
#include <sstream>

MovePictureCommand::MovePictureCommand(std::stringstream& input)
{
	if (!(input >> m_dx >> m_dy))
	{
		throw std::invalid_argument("Invalid AddShape syntax");
	}
}

void MovePictureCommand::Execute(Picture& picture, ICanvas& canvas)
{
	picture.MovePicture(Point{ m_dx, m_dy });
}