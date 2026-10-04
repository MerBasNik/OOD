//
// Created by Вадим Патрушев on 28.09.2026.
//

#include "Canvas.h"
#include "Picture.h"
#include "Parser/Parser.h"
#include <iostream>
#include <sstream>

int main()
{
	Canvas canvas;
	Picture picture;

	try
	{
		Parser parser;
		std::string line;
		while (getline(std::cin, line))
		{
			std::stringstream input(line);
			auto command = parser.ParseCommand(input);
			command->Execute(picture, canvas);
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}