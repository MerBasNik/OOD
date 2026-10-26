//
// Created by Вадим Патрушев on 15.10.2026.
//

#include "EditorWindow.h"
#include "FileWindows.h"
#include "View/ShapeIcons.h"

#include <list>

const sf::Color WORKSPACE_COLOR(210, 210, 210);
const char* DEFAULT_FILE_NAME = "editor.draw";

EditorWindow::EditorWindow(Document& document, EditorState& state, EditorController& controller,
	ShapeListController& shapeListController)
	: m_state(state)
	, m_controller(controller)
	, m_window(sf::VideoMode({
				   static_cast<unsigned>(document.GetBounds().m_width + ShapeListView::WIDTH),
				   static_cast<unsigned>(document.GetBounds().m_height + Toolbar::HEIGHT),
			   }),
		  "Editor")
	, m_canvasView(document, state, { 0.f, Toolbar::HEIGHT })
	, m_shapeListView(document, state, shapeListController,
		  sf::FloatRect({ static_cast<float>(document.GetBounds().m_width), Toolbar::HEIGHT },
			  { ShapeListView::WIDTH, static_cast<float>(document.GetBounds().m_height) }))
{
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
		if (m_window.isOpen() && (m_canvasView.IsChange() || m_shapeListView.IsChange()))
		{
			Render();
		}
	}
}

void EditorWindow::InitToolbar()
{
	const std::list<ShapeType> types = { ShapeType::Rectangle, ShapeType::Ellipse, ShapeType::Triangle };
	for (const auto type : types)
	{
		m_toolbar.AddIconButton(ShapeIcons::Load(type), [this, type] {
			m_controller.SelectShapeTool(m_state.GetShapeTool() == type ? std::nullopt : std::optional(type));
		}, [this, type] { return m_state.GetShapeTool() == type; });
	}
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
	const std::pair<CursorType, sf::Cursor::Type> cursors[] = {
		{ CursorType::Arrow, sf::Cursor::Type::Arrow },
		{ CursorType::Move, sf::Cursor::Type::SizeAll },
		{ CursorType::SizeHorizontal, sf::Cursor::Type::SizeHorizontal },
		{ CursorType::SizeVertical, sf::Cursor::Type::SizeVertical },
		{ CursorType::SizeTopLeftBottomRight, sf::Cursor::Type::SizeTopLeftBottomRight },
		{ CursorType::SizeBottomLeftTopRight, sf::Cursor::Type::SizeBottomLeftTopRight },
		{ CursorType::Cross, sf::Cursor::Type::Cross },
	};
	for (const auto& [type, sfType] : cursors)
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
		m_canvasView.SetIsChange();
	}
	else if (event.is<sf::Event::FocusGained>())
	{
		m_canvasView.SetIsChange();
	}
	else if (event.is<sf::Event::FocusLost>())
	{
		HandleMouseReleased(m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window)));
	}
	else if (const auto* pressed = event.getIf<sf::Event::MouseButtonPressed>())
	{
		if (pressed->button == sf::Mouse::Button::Left)
		{
			HandleMousePressed(m_window.mapPixelToCoords(pressed->position));
		}
	}
	else if (const auto* released = event.getIf<sf::Event::MouseButtonReleased>())
	{
		if (released->button == sf::Mouse::Button::Left)
		{
			HandleMouseReleased(m_window.mapPixelToCoords(released->position));
		}
	}
	else if (const auto* moved = event.getIf<sf::Event::MouseMoved>())
	{
		HandleMouseMoved(m_window.mapPixelToCoords(moved->position));
	}
	else if (const auto* scrolled = event.getIf<sf::Event::MouseWheelScrolled>())
	{
		m_shapeListView.OnMouseWheel(m_window.mapPixelToCoords(scrolled->position), scrolled->delta);
	}
	else if (const auto* key = event.getIf<sf::Event::KeyPressed>())
	{
		HandleKeyPressed(*key);
	}
}

void EditorWindow::HandleMousePressed(const sf::Vector2f point)
{
	if (m_toolbar.HandleClick(point))
	{
		m_mouseCapture = MouseCapture::None;
	}
	else if (m_shapeListView.IsContains(point))
	{
		m_mouseCapture = MouseCapture::ShapeList;
		m_shapeListView.OnMouseDown(point);
	}
	else
	{
		m_mouseCapture = MouseCapture::Canvas;
		m_controller.OnMouseDown(m_canvasView.ToDocumentPoint(point));
	}
	UpdateCursor(point);
}

void EditorWindow::HandleMouseMoved(const sf::Vector2f point)
{
	switch (m_mouseCapture)
	{
	case MouseCapture::Canvas:
		m_controller.OnMouseMove(m_canvasView.ToDocumentPoint(point));
		break;
	case MouseCapture::ShapeList:
		m_shapeListView.OnMouseMove(point);
		break;
	case MouseCapture::None:
		break;
	}
	UpdateCursor(point);
}

void EditorWindow::HandleMouseReleased(const sf::Vector2f point)
{
	switch (m_mouseCapture)
	{
	case MouseCapture::Canvas:
		m_controller.OnMouseUp(m_canvasView.ToDocumentPoint(point));
		break;
	case MouseCapture::ShapeList:
		m_shapeListView.OnMouseUp(point);
		break;
	case MouseCapture::None:
		break;
	}
	m_mouseCapture = MouseCapture::None;
	UpdateCursor(point);
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
		case sf::Keyboard::Key::Z:
			key.shift ? Redo() : Undo();
			break;
		case sf::Keyboard::Key::Y:
			Redo();
			break;
		default:
			break;
		}
		return;
	}
	if (key.code == sf::Keyboard::Key::Escape)
	{
		m_shapeListView.CancelDrag();
		m_controller.Cancel();
		UpdateCursor(m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window)));
	}
	else if (key.code == sf::Keyboard::Key::Delete || key.code == sf::Keyboard::Key::Backspace)
	{
		m_controller.DeleteSelectedShape();
	}
}

void EditorWindow::UpdateCursor(const sf::Vector2f point)
{
	const bool isCanvasCursor = m_mouseCapture == MouseCapture::Canvas
		|| (m_mouseCapture == MouseCapture::None && !m_toolbar.IsContains(point) && !m_shapeListView.IsContains(point));
	const CursorType type = isCanvasCursor
		? m_controller.GetCursorType(m_canvasView.ToDocumentPoint(point))
		: CursorType::Arrow;
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
	m_shapeListView.Draw(m_window);
	m_toolbar.Draw(m_window);
	m_window.display();
	m_canvasView.SetDontChange();
	m_shapeListView.SetDontChange();
}

void EditorWindow::NewDocument()
{
	m_controller.NewDocument();
}

void EditorWindow::Undo()
{
	m_shapeListView.CancelDrag();
	m_controller.Undo();
}

void EditorWindow::Redo()
{
	m_shapeListView.CancelDrag();
	m_controller.Redo();
}

void EditorWindow::OpenDocument()
{
	if (const auto path = FileWindows::ShowOpenFileWindow())
	{
		RunFileOperation("Не удалось открыть документ.", [&] { m_controller.OpenDocument(*path); });
	}
}

void EditorWindow::SaveDocument()
{
	auto path = m_state.GetFilePath();
	if (!path)
	{
		path = FileWindows::ShowSaveFileWindow(DEFAULT_FILE_NAME);
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
	m_canvasView.SetIsChange();
}
