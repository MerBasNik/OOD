//
// Created by Вадим Патрушев on 08.11.2026.
//

#include "AppWindow.h"

AppWindow::AppWindow(std::shared_ptr<AppDocument> appDocument, IApplication& application)
	: m_appDocument(std::move(appDocument))
	, m_controller(m_appDocument->GetDocument(), m_state, m_appDocument->GetCommandHistory())
	, m_shapeListController(m_appDocument->GetDocument(), m_state, m_appDocument->GetCommandHistory())
	, m_timelineController(m_appDocument->GetDocument(), m_state, m_appDocument->GetCommandHistory())
	, m_editorWindow(application, m_appDocument, m_state, m_controller, m_shapeListController, m_timelineController)
{
}

bool AppWindow::IsOpen() const
{
	return m_editorWindow.IsOpen();
}

void AppWindow::ProcessEvents()
{
	m_editorWindow.ProcessEvents();
}

void AppWindow::RenderWindow()
{
	m_editorWindow.RenderWindow();
}
