//
// Created by Вадим Патрушев on 28.09.2026.
//

#include "Canvas.h"
#include "Parser/Parser.h"
#include "Picture.h"
#include "Observer/PictureObserver.h"

#include <iostream>
#include <sstream>

const unsigned CANVAS_WIDTH = 800;
const unsigned CANVAS_HEIGHT = 600;

int main()
{
	Canvas canvas(CANVAS_WIDTH, CANVAS_HEIGHT);
	PictureObserver pictureObserver;
	Picture picture;
	picture.AddObserver(pictureObserver);
	Parser parser;

	try
	{
		std::string line;
		canvas.HandleClose();
		while (getline(std::cin, line))
		{
			std::stringstream input(line);
			const auto command = parser.ParseCommand(input);
			command->Execute(picture, canvas);
			// TODO перенести display canvas внутрь команд draw
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}