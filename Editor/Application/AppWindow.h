//
// Created by Вадим Патрушев on 08.11.2026.
//

#ifndef OOD_APPWINDOW_H
#define OOD_APPWINDOW_H
#include "IApplication.h"
#include "Controller/EditorController.h"
#include "Controller/ShapeListController.h"
#include "Model/EditorState/EditorState.h"
#include "View/Window/EditorWindow.h"


#include <memory>



class AppWindow {
public:
	AppWindow(std::shared_ptr<AppDocument> appDocument, IApplication& application, const std::string& appName);
	AppWindow(const AppWindow&) = delete;
	AppWindow& operator=(const AppWindow&) = delete;

	bool IsOpen() const;
	void ProcessEvents();
	void RenderWindow();

private:
	std::shared_ptr<AppDocument> m_appDocument;
	EditorState m_state;
	EditorController m_controller;
	ShapeListController m_shapeListController;
	EditorWindow m_editorWindow;
};



#endif //OOD_APPWINDOW_H
