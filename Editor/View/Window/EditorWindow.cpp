//
// Created by Вадим Патрушев on 15.10.2026.
//

#include "EditorWindow.h"
#include "FileWindows.h"
#include "View/ShapeIcons.h"
#include <list>

const sf::Color WORKSPACE_COLOR(210, 210, 210);
const char* DEFAULT_FILE_NAME = "editor.draw";

EditorWindow::EditorWindow(
	IApplication& application,
	std::shared_ptr<AppDocument> appDocument,
	EditorState& state,
	EditorController& controller,
	ShapeListController& shapeListController,
	TimelineController& timelineController
)
	: m_application(application)
	, m_appDocument(std::move(appDocument))
	, m_document(m_appDocument->GetDocument())
	, m_windowNumber(m_appDocument->NextWindowNumber())
	, m_title(GetTitle())
	, m_state(state)
	, m_controller(controller)
	, m_window(sf::VideoMode({
					static_cast<unsigned>(m_document.GetBounds().m_width + ShapeListView::WIDTH),
					static_cast<unsigned>(m_document.GetBounds().m_height + Toolbar::HEIGHT + TimelineView::HEIGHT),
				}),
				sf::String::fromUtf8(m_title.begin(), m_title.end()))
	, m_colorPicker(m_toolbar.GetFont(), [this] { m_controller.CommitColors(); }, [this] { m_controller.CancelColors(); })
	, m_canvasView(m_document, state, { 0.f, Toolbar::HEIGHT })
	, m_shapeListView(m_document, state, shapeListController,
		sf::FloatRect({ static_cast<float>(m_document.GetBounds().m_width), Toolbar::HEIGHT },
	{ ShapeListView::WIDTH, static_cast<float>(m_document.GetBounds().m_height) }))
	, m_timelineView(m_document, state, timelineController,
		sf::FloatRect({ 0.f, Toolbar::HEIGHT + static_cast<float>(m_document.GetBounds().m_height) },
	{ static_cast<float>(m_document.GetBounds().m_width) + ShapeListView::WIDTH, TimelineView::HEIGHT }))
{
	InitToolbar();
	InitCursors();
}

void EditorWindow::ProcessEvents()
{
	bool hasEvents = false;
	while (const auto event = m_window.pollEvent())
	{
		hasEvents = true;
		HandleEvent(*event);
	}
	if (!hasEvents && m_mouseCapture != MouseCapture::None)
	{
		TrackMouse();
	}
	UpdateTitle();
}

void EditorWindow::RenderWindow()
{
	if (IsOpen() && (m_canvasView.IsChange() || m_shapeListView.IsChange() || m_timelineView.IsChange()))
	{
		Render();
	}
}

bool EditorWindow::IsOpen() const
{
	return m_window.isOpen();
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
	m_fillButtonRect = m_toolbar.AddButton("Заливка", [this] {
		OpenColorPicker("Цвет заливки", m_fillButtonRect, true);
	});
	m_borderButtonRect = m_toolbar.AddButton("Контур", [this] {
		OpenColorPicker("Цвет контура", m_borderButtonRect, false);
	});
	m_toolbar.AddButton("Удалить", [this] {
		CancelColorPicker();
		m_controller.DeleteSelectedShapes();
	});
	m_toolbar.AddButton("Новое окно", [this] {
		NewWindow();
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

void EditorWindow::OpenColorPicker(const std::string& title, const sf::FloatRect buttonRect, const bool isFill)
{
	const auto& selectedIds = m_state.GetSelectedShapeIds();
	const IShape* shape = selectedIds.empty() ? nullptr : m_document.FindShape(selectedIds.front());
	if (!shape)
	{
		return;
	}
	const sf::Vector2f position(buttonRect.position.x, Toolbar::HEIGHT);
	if (isFill)
	{
		m_colorPicker.Open(title, position, shape->GetFillColor(), [this](const Color color) {
			m_controller.PreviewFillColor(color);
		});
	}
	else
	{
		m_colorPicker.Open(title, position, shape->GetBorderColor(), [this](const Color color) {
			m_controller.PreviewBorderColor(color);
		});
	}
	m_canvasView.SetIsChange();
}

void EditorWindow::CancelColorPicker()
{
	if (m_colorPicker.IsOpen())
	{
		m_colorPicker.Cancel();
		m_canvasView.SetIsChange();
	}
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
		m_isTogglePressed = false;
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
		const sf::Vector2f point = m_window.mapPixelToCoords(scrolled->position);
		m_shapeListView.OnMouseWheel(point, scrolled->delta);
		m_timelineView.OnMouseWheel(point, scrolled->delta);
	}
	else if (const auto* key = event.getIf<sf::Event::KeyPressed>())
	{
		m_isTogglePressed = key->shift || key->control || key->system || IsToggleModifierKey(key->code);
		HandleKeyPressed(*key);
	}
	else if (const auto* released = event.getIf<sf::Event::KeyReleased>())
	{
		using Key = sf::Keyboard::Key;
		const bool isShift = released->shift && released->code != Key::LShift && released->code != Key::RShift;
		const bool isControl = released->control && released->code != Key::LControl && released->code != Key::RControl;
		const bool isSystem = released->system && released->code != Key::LSystem && released->code != Key::RSystem;
		m_isTogglePressed = isShift || isControl || isSystem;
	}
}

void EditorWindow::TrackMouse()
{
	const sf::Vector2f point = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		HandleMouseMoved(point);
	}
	else
	{
		HandleMouseReleased(point);
	}
}

void EditorWindow::SkipInputEvents()
{
	while (const auto event = m_window.pollEvent())
	{
		if (!event->is<sf::Event::MouseButtonPressed>() && !event->is<sf::Event::MouseButtonReleased>()
			&& !event->is<sf::Event::KeyPressed>())
		{
			HandleEvent(*event);
		}
	}
}

bool EditorWindow::IsToggleModifierKey(const sf::Keyboard::Key code)
{
	using Key = sf::Keyboard::Key;
	return code == Key::LShift || code == Key::RShift || code == Key::LControl || code == Key::RControl
		|| code == Key::LSystem || code == Key::RSystem;
}

void EditorWindow::HandleMousePressed(const sf::Vector2f point)
{
	if (m_colorPicker.HandleClick(point))
	{
		m_mouseCapture = MouseCapture::None;
		m_canvasView.SetIsChange();
		UpdateCursor(point);
		return;
	}
	CancelColorPicker();

	if (m_toolbar.HandleClick(point))
	{
		m_mouseCapture = MouseCapture::None;
	}
	else if (m_shapeListView.IsContains(point))
	{
		m_mouseCapture = MouseCapture::ShapeList;
		m_shapeListView.OnMouseDown(point, m_isTogglePressed);
	}
	else if (m_timelineView.IsContains(point))
	{
		m_mouseCapture = MouseCapture::Timeline;
		m_timelineView.OnMouseDown(point, m_isTogglePressed);
	}
	else
	{
		m_mouseCapture = MouseCapture::Canvas;
		m_controller.OnMouseDown(m_canvasView.ToDocumentPoint(point), m_isTogglePressed);
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
	case MouseCapture::Timeline:
		m_timelineView.OnMouseMove(point);
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
	case MouseCapture::Timeline:
		m_timelineView.OnMouseUp(point);
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
			key.shift ? NewWindow() : NewDocument();
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
		if (m_colorPicker.IsOpen())
		{
			CancelColorPicker();
			return;
		}
		m_shapeListView.CancelDrag();
		m_timelineView.CancelDrag();
		m_controller.Cancel();
		UpdateCursor(m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window)));
	}
	else if (key.code == sf::Keyboard::Key::Delete || key.code == sf::Keyboard::Key::Backspace)
	{
		CancelColorPicker();
		m_controller.DeleteSelectedShapes();
	}
}

void EditorWindow::UpdateCursor(const sf::Vector2f point)
{
	const bool isFree = m_mouseCapture == MouseCapture::None && !m_colorPicker.IsContains(point);
	const bool isTimelineCursor = m_mouseCapture == MouseCapture::Timeline
		|| (isFree && m_timelineView.IsContains(point));
	const bool isCanvasCursor = m_mouseCapture == MouseCapture::Canvas
		|| (isFree && !m_toolbar.IsContains(point) && !m_shapeListView.IsContains(point)
			&& !m_timelineView.IsContains(point));
	CursorType type = CursorType::Arrow;
	if (isTimelineCursor)
	{
		type = m_timelineView.GetCursorType(point);
	}
	else if (isCanvasCursor)
	{
		type = m_controller.GetCursorType(m_canvasView.ToDocumentPoint(point));
	}
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
	m_timelineView.Draw(m_window);
	m_toolbar.Draw(m_window);
	m_colorPicker.Draw(m_window);
	m_window.display();
	m_canvasView.SetDontChange();
	m_shapeListView.SetDontChange();
	m_timelineView.SetDontChange();
}

void EditorWindow::NewDocument()
{
	CancelColorPicker();
	m_application.NewApplication();
}

void EditorWindow::NewWindow()
{
	CancelColorPicker();
	m_application.NewWindow(m_appDocument);
}

void EditorWindow::UpdateTitle()
{
	std::string title = GetTitle();
	if (title != m_title)
	{
		m_title = std::move(title);
		m_window.setTitle(sf::String::fromUtf8(m_title.begin(), m_title.end()));
	}
}

std::string EditorWindow::GetTitle() const
{
	std::string title = m_appDocument->GetName();
	if (const auto path = m_appDocument->GetFilePath())
	{
		title += " (" + path->filename().string() + ")";
	}
	return title + " — окно " + std::to_string(m_windowNumber);
}

void EditorWindow::Undo()
{
	CancelColorPicker();
	m_shapeListView.CancelDrag();
	m_timelineView.CancelDrag();
	m_controller.Undo();
}

void EditorWindow::Redo()
{
	CancelColorPicker();
	m_shapeListView.CancelDrag();
	m_timelineView.CancelDrag();
	m_controller.Redo();
}

void EditorWindow::OpenDocument()
{
	CancelColorPicker();
	if (const auto path = FileWindows::ShowOpenFileWindow())
	{
		RunFileOperation("Не удалось открыть документ.", [&] { m_application.OpenApplication(*path); });
	}
	SkipInputEvents();
}

void EditorWindow::SaveDocument()
{
	auto path = m_appDocument->GetFilePath();
	if (!path)
	{
		path = FileWindows::ShowSaveFileWindow(DEFAULT_FILE_NAME);
	}
	if (path)
	{
		RunFileOperation("Не удалось сохранить документ.", [&] { m_appDocument->Save(*path); });
	}
	SkipInputEvents();
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
