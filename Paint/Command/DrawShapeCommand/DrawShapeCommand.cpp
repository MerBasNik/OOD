//
// Created by Вадим Патрушев on 04.10.2026.
//

#include "DrawShapeCommand.h"
#include <sstream>

DrawShapeCommand::DrawShapeCommand(std::stringstream& input)
{
	if (!(input >> m_id))
	{
		throw std::invalid_argument("Invalid input");
	}
}

void DrawShapeCommand::Execute(Picture& picture, ICanvas& canvas)
{
	picture.DrawShape(m_id, canvas);
}