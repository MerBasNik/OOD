//
// Created by Вадим Патрушев on 15.10.2026.
//

#include "Controller/EditorController.h"
#include "Controller/ShapeListController.h"
#include "View/Window/EditorWindow.h"
#include <iostream>

const double CANVAS_WIDTH = 1000;
const double CANVAS_HEIGHT = 650;
// TODO разобраться MVC или MVP и даграмму
// TODO должно ли храниться выделение в модели
// TODO подлагивания

int main()
{
	try
	{
		Document document(CANVAS_WIDTH, CANVAS_HEIGHT);
		EditorState state;
		EditorController controller(document, state);
		ShapeListController shapeListController(document, state);
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
