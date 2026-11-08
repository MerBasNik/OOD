//
// Created by Вадим Патрушев on 02.11.2026.
//

#include "Application.h"

constexpr double CANVAS_WIDTH = 1000;
constexpr double CANVAS_HEIGHT = 650;

int Application::Run()
{
	NewApplication();
	while (true)
	{
		AddNewWindows();
		if (m_appWindows.empty())
		{
			return 0;
		}
		for (const auto& appWindow : m_appWindows)
		{
			appWindow->ProcessEvents();
		}
		for (const auto& appWindow : m_appWindows)
		{
			appWindow->RenderWindow();
		}
		for (auto it = m_appWindows.begin(); it != m_appWindows.end();)
		{
			if ((*it)->IsOpen())
			{
				++it;
			}
			else
			{
				it = m_appWindows.erase(it);
			}
		}
	}
}

void Application::NewApplication()
{
	NewWindow(std::make_shared<AppDocument>(CANVAS_WIDTH, CANVAS_HEIGHT, NextDocumentName()));
}

void Application::OpenApplication(const std::filesystem::path& path)
{
	NewWindow(AppDocument::Load(path, CANVAS_WIDTH, CANVAS_HEIGHT, NextDocumentName()));
}

void Application::NewWindow(std::shared_ptr<AppDocument> appDocument)
{
	m_newAppWindows.push_back(std::make_unique<AppWindow>(std::move(appDocument), *this));
}

void Application::AddNewWindows()
{
	for (auto& appWindow : m_newAppWindows)
	{
		m_appWindows.push_back(std::move(appWindow));
	}
	m_newAppWindows.clear();
}

std::string Application::NextDocumentName()
{
	return "Документ " + std::to_string(++m_documentCount);
}
