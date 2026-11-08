//
// Created by Вадим Патрушев on 15.10.2026.
//

#include "Application/Application.h"

#include <iostream>

// TODO отслеживать позицию курсора за пределами экрана
// TODO ограничить число открытий экрана сохранения
// TODO посмотреть каких тестов не хватает

// TODO добавить визуальное выделение кнопок
// TODO скрывать кнопки, когда фигуры не выделены

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
