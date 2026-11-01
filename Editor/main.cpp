//
// Created by Вадим Патрушев on 15.10.2026.
//

#include "Controller/EditorController.h"
#include "Controller/ShapeListController.h"
#include "View/Window/EditorWindow.h"
#include <iostream>

const double CANVAS_WIDTH = 1000;
const double CANVAS_HEIGHT = 650;
// TODO отслеживать позицию курсора за пределами экрана, чтобы при перемещении курсора за экраном, фигура на холсте тоже перемещалась
// TODO ограничить число открытий экрана сохранения, чтобы при нескольких кликах на кнопку, окно открывалось только один раз
// TODO посмотреть каких тестов не хватает

int main()
{
	try
	{
		Document document(CANVAS_WIDTH, CANVAS_HEIGHT);
		EditorState state;
		CommandHistory history;
		EditorController controller(document, state, history);
		ShapeListController shapeListController(document, state, history);
		EditorWindow window(document, state, controller, shapeListController);
		window.Run();
		return 0;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
}
