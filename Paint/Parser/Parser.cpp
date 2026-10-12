//
// Created by Вадим Патрушев on 04.10.2026.
//

#include "Parser.h"
#include "Command/AddShapeCommand/AddShapeCommand.h"
#include "Command/ChangeColorCommand/ChangeColorCommand.h"
#include "Command/ChangeShapeCommand/ChangeShapeCommand.h"
#include "Command/CloneShapeCommand/CloneShapeCommand.h"
#include "Command/DeleteShapeCommand/DeleteShapeCommand.h"
#include "Command/DrawPictureCommand/DrawPictureCommand.h"
#include "Command/DrawShapeCommand/DrawShapeCommand.h"
#include "Command/ListCommand/ListCommand.h"
#include "Command/MovePictureCommand/MovePictureCommand.h"
#include "Command/MoveShapeCommand/MoveShapeCommand.h"
#include <sstream>
#include <string>

std::unique_ptr<ICommand> Parser::ParseCommand(std::stringstream& input)
{
	std::string command;
	input >> command;
	if (command == "AddShape")
	{
		return std::make_unique<AddShapeCommand>(input);
	}
	if (command == "MoveShape")
	{
		return std::make_unique<MoveShapeCommand>(input);
	}
	if (command == "MovePicture")
	{
		return std::make_unique<MovePictureCommand>(input);
	}
	if (command == "ChangeShape")
	{
		return std::make_unique<ChangeShapeCommand>(input);
	}
	if (command == "ChangeColor")
	{
		return std::make_unique<ChangeColorCommand>(input);
	}
	if (command == "DeleteShape")
	{
		return std::make_unique<DeleteShapeCommand>(input);
	}
	if (command == "DrawShape")
	{
		return std::make_unique<DrawShapeCommand>(input);
	}
	if (command == "DrawPicture")
	{
		return std::make_unique<DrawPictureCommand>();
	}
	if (command == "List")
	{
		return std::make_unique<ListCommand>();
	}
	if (command == "CloneShape")
	{
		return std::make_unique<CloneShapeCommand>(input);
	}

	throw std::invalid_argument("Неизвестная команда: " + command);
}