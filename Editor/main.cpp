//
// Created by Вадим Патрушев on 15.10.2026.
//

#include "Application/Application.h"

#include <iostream>

// TODO добавить визуальное выделение кнопок
// TODO скрывать кнопки, когда фигуры не выделены
// TODO исправить баг: при создании фигуры с минимальными размерами после уменьшения размеров она прыгает

int main()
{
	try
	{
		Application app;
		return app.Run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
}
