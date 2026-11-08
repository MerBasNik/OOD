//
// Created by Вадим Патрушев on 02.11.2026.
//

#include "Application.h"
#include <iostream>

Application::Application()
{
}

void Application::NewApplication()
{
	const auto appDocument = std::make_shared<AppDocument>(CANVAS_WIDTH, CANVAS_HEIGHT);
	appDocument->NewWindow();
	m_appCount++;
}

void Application::OpenApplication(const std::filesystem::path& path)
{
	appDocument->Load(path);
	appDocument->OpenWindow();
}

void Application::NewWindow(std::shared_ptr<AppDocument> appDocument)
{
	m_appWindows.push_back(appDocument);
}

int Application::Run()
{
	while (!m_appWindows.empty())
	{
		for (auto appDocument : m_appWindows)
		{
			appDocument.ProcessEvents();
			appDocument.RenderWindow();
		}
	}
	return 0;
}