//
// Created by Вадим Патрушев on 15.10.2026.
//

#include "Application/Application.h"
#include <iostream>

// TODO разделить на отдельные сущности: фигуры и анимации
// TODO добавить кнопки +- и проценты зума
// TODO дополнить тесты на ресайз

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
