//
// Created by Вадим Патрушев on 15.10.2026.
//

#include "Controller/EditorController.h"
#include "View/EditorWindow.h"
#include <iostream>

const double CANVAS_WIDTH = 1000;
const double CANVAS_HEIGHT = 650;

int main()
{
	try
	{
		Document document(CANVAS_WIDTH, CANVAS_HEIGHT);
		EditorState state;
		EditorController controller(document, state);
		EditorWindow window(document, state, controller);
		window.Run();
		return 0;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
}
