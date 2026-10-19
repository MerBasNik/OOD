//
// Created by Вадим Патрушев on 15.10.2026.
//

#include "EditorWindow.h"
#include "FileWindows.h"

const sf::Color WORKSPACE_COLOR(210, 210, 210);
const char* DEFAULT_FILE_NAME = "editor.draw";

EditorWindow::EditorWindow(Document& document, EditorState& state, EditorController& controller)
	: m_state(state)
	, m_controller(controller)
	, m_window(sf::VideoMode({
				   static_cast<unsigned>(document.GetBounds().m_width),
				   static_cast<unsigned>(document.GetBounds().m_height + Toolbar::HEIGHT),
			   }),
		  "Editor")
	, m_canvasView(document, state, { 0.f, Toolbar::HEIGHT })
{
	m_window.setVerticalSyncEnabled(true);
	InitToolbar();
	InitCursors();
}

void EditorWindow::Run()
{
	while (m_window.isOpen())
	{
		if (const auto event = m_window.waitEvent())
		{
			HandleEvent(*event);
		}
		while (const auto event = m_window.pollEvent())
		{
			HandleEvent(*event);
		}
		if (m_window.isOpen() && m_canvasView.IsInvalid())
		{
			Render();
		}
	}
}

void EditorWindow::InitToolbar()
{
	m_toolbar.AddButton("Прямоугольник", [this] {
		m_controller.AddShape(ShapeType::Rectangle);
	});
	m_toolbar.AddButton("Эллипс", [this] {
		m_controller.AddShape(ShapeType::Ellipse);
	});
	m_toolbar.AddButton("Треугольник", [this] {
		m_controller.AddShape(ShapeType::Triangle);
	});
	m_toolbar.AddButton("Удалить", [this] {
		m_controller.DeleteSelectedShape();
	});
	m_toolbar.AddSeparator();
	m_toolbar.AddButton("Новый", [this] {
		NewDocument();
	});
	m_toolbar.AddButton("Открыть", [this] {
		OpenDocument();
	});
	m_toolbar.AddButton("Сохранить", [this] {
		SaveDocument();
	});
}

void EditorWindow::InitCursors()
{
	const std::pair<CursorType, sf::Cursor::Type> mapping[] = {
		{ CursorType::Arrow, sf::Cursor::Type::Arrow },
		{ CursorType::Move, sf::Cursor::Type::SizeAll },
		{ CursorType::SizeHorizontal, sf::Cursor::Type::SizeHorizontal },
		{ CursorType::SizeVertical, sf::Cursor::Type::SizeVertical },
		{ CursorType::SizeTopLeftBottomRight, sf::Cursor::Type::SizeTopLeftBottomRight },
		{ CursorType::SizeBottomLeftTopRight, sf::Cursor::Type::SizeBottomLeftTopRight },
	};
	for (const auto& [type, sfType] : mapping)
	{
		if (auto cursor = sf::Cursor::createFromSystem(sfType))
		{
			m_cursors.emplace(type, std::move(*cursor));
		}
	}
}

void EditorWindow::HandleEvent(const sf::Event& event)
{
	if (event.is<sf::Event::Closed>())
	{
		m_window.close();
	}
	else if (const auto* resized = event.getIf<sf::Event::Resized>())
	{
		m_window.setView(sf::View(sf::FloatRect({ 0.f, 0.f }, sf::Vector2f(resized->size))));
		m_canvasView.Invalidate();
	}
	else if (event.is<sf::Event::FocusGained>())
	{
		m_canvasView.Invalidate();
	}
	else if (event.is<sf::Event::FocusLost>())
	{
		if (m_isMousePressed)
		{
			m_isMousePressed = false;
			m_controller.OnMouseUp(ToDocumentPoint(sf::Mouse::getPosition(m_window)));
		}
	}
	else if (const auto* pressed = event.getIf<sf::Event::MouseButtonPressed>())
	{
		if (pressed->button != sf::Mouse::Button::Left)
		{
			return;
		}
		const sf::Vector2f point = m_window.mapPixelToCoords(pressed->position);
		if (!m_toolbar.HandleClick(point))
		{
			m_isMousePressed = true;
			m_controller.OnMouseDown(m_canvasView.ToDocumentPoint(point));
		}
		UpdateCursor(point);
	}
	else if (const auto* released = event.getIf<sf::Event::MouseButtonReleased>())
	{
		if (released->button == sf::Mouse::Button::Left && m_isMousePressed)
		{
			m_isMousePressed = false;
			m_controller.OnMouseUp(ToDocumentPoint(released->position));
		}
		UpdateCursor(m_window.mapPixelToCoords(released->position));
	}
	else if (const auto* moved = event.getIf<sf::Event::MouseMoved>())
	{
		if (m_isMousePressed)
		{
			m_controller.OnMouseMove(ToDocumentPoint(moved->position));
		}
		UpdateCursor(m_window.mapPixelToCoords(moved->position));
	}
	else if (const auto* key = event.getIf<sf::Event::KeyPressed>())
	{
		HandleKeyPressed(*key);
	}
}

void EditorWindow::HandleKeyPressed(const sf::Event::KeyPressed& key)
{
	const bool isCommand = key.control || key.system;
	if (isCommand)
	{
		switch (key.code)
		{
		case sf::Keyboard::Key::S:
			SaveDocument();
			break;
		case sf::Keyboard::Key::O:
			OpenDocument();
			break;
		case sf::Keyboard::Key::N:
			NewDocument();
			break;
		default:
			break;
		}
		return;
	}
	if (key.code == sf::Keyboard::Key::Delete || key.code == sf::Keyboard::Key::Backspace)
	{
		m_controller.DeleteSelectedShape();
	}
}

void EditorWindow::UpdateCursor(const sf::Vector2f point)
{
	const CursorType type = !m_isMousePressed && m_toolbar.IsContains(point)
		? CursorType::Arrow
		: m_controller.GetCursorType(m_canvasView.ToDocumentPoint(point));
	if (type == m_currentCursor)
	{
		return;
	}
	const auto it = m_cursors.find(type);
	const auto fallback = m_cursors.find(CursorType::Arrow);
	if (it != m_cursors.end())
	{
		m_window.setMouseCursor(it->second);
	}
	else if (fallback != m_cursors.end())
	{
		m_window.setMouseCursor(fallback->second);
	}
	m_currentCursor = type;
}

void EditorWindow::Render()
{
	m_window.clear(WORKSPACE_COLOR);
	m_canvasView.Draw(m_window);
	m_toolbar.Draw(m_window);
	m_window.display();
	m_canvasView.Validate();
}

void EditorWindow::NewDocument()
{
	m_controller.NewDocument();
}

void EditorWindow::OpenDocument()
{
	if (const auto path = FileWindows::ShowOpenFileDialog())
	{
		RunFileOperation("Не удалось открыть документ.", [&] { m_controller.OpenDocument(*path); });
	}
}

void EditorWindow::SaveDocument()
{
	auto path = m_state.GetFilePath();
	if (!path)
	{
		path = FileWindows::ShowSaveFileDialog(DEFAULT_FILE_NAME);
	}
	if (path)
	{
		RunFileOperation("Не удалось сохранить документ.", [&] { m_controller.SaveDocument(*path); });
	}
}

void EditorWindow::RunFileOperation(const std::string& errorMessage, const std::function<void()>& operation)
{
	try
	{
		operation();
	}
	catch (const std::exception& e)
	{
		FileWindows::ShowError(errorMessage + "\n" + e.what());
	}
	m_canvasView.Invalidate();
}

Point EditorWindow::ToDocumentPoint(const sf::Vector2i pixel) const
{
	return m_canvasView.ToDocumentPoint(m_window.mapPixelToCoords(pixel));
}
