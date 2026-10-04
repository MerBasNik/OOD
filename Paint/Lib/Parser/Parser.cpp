//
// Created by Вадим Патрушев on 04.10.2026.
//

#include "Parser.h"
#include "Command/AddCommand/AddCommand.h"
#include "Command/ChangeColor/ChangeColor.h"
#include "Command/ChangeCommand/ChangeCommand.h"
#include "Command/CloneCommand/CloneCommand.h"
#include "Command/DeleteCommand/DeleteCommand.h"
#include "Command/DrawCommand/DrawCommand.h"
#include "Command/DrawPictureCommand/DrawPictureCommand.h"
#include "Command/ListCommand/ListCommand.h"
#include "Command/MoveCommand/MoveCommand.h"
#include "Command/MovePictureCommand/MovePictureCommand.h"
#include <sstream>
#include <string>

std::unique_ptr<ICommand> Parser::ParseCommand(std::stringstream& input)
{
	std::string command;
	input >> command;
	if (command == "AddShape")
	{
		return std::make_unique<AddCommand>(input);
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
		return std::make_unique<ChangeCommand>(input);
	}
	if (command == "ChangeColor")
	{
		return std::make_unique<ChangeColor>(input);
	}
	if (command == "DeleteShape")
	{
		return std::make_unique<DeleteCommand>(input);
	}
	if (command == "DrawShape")
	{
		return std::make_unique<DrawCommand>(input);
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
		return std::make_unique<CloneCommand>(input);
	}

	throw std::invalid_argument("undefined command: " + command);
}