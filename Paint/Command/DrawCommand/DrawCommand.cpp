//
// Created by Вадим Патрушев on 04.10.2026.
//

#include "DrawCommand.h"
#include <sstream>

DrawCommand::DrawCommand(std::stringstream& input)
{
	if (!(input >> m_id))
	{
		throw std::invalid_argument("Invalid input");
	}
}

void DrawCommand::Execute(Picture& picture, ICanvas& canvas)
{
	picture.DrawShape(m_id, canvas);
}