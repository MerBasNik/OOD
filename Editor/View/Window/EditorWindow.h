//
// Created by Вадим Патрушев on 15.10.2026.
//

#ifndef OOD_EDITOR_EDITORWINDOW_H
#define OOD_EDITOR_EDITORWINDOW_H

#include "Application/IApplication.h"
#include "ColorPicker.h"
#include "Controller/EditorController.h"
#include "Enums/MouseCapture.h"
#include "Toolbar.h"
#include "View/Canvas/CanvasView.h"
#include "View/ShapeList/ShapeListView.h"
#include "View/Timeline/TimelineView.h"
#include <SFML/Graphics.hpp>
#include <functional>
#include <map>
#include <memory>
#include <string>

class EditorWindow
{
public:
	EditorWindow(
		IApplication& application,
		std::shared_ptr<AppDocument> appDocument,
		EditorState& state,
		EditorController& controller,
		ShapeListController& shapeListController,
		TimelineController& timelineController);
	void ProcessEvents();
	void RenderWindow();
	bool IsOpen() const;

private:
	IApplication& m_application;
	std::shared_ptr<AppDocument> m_appDocument;
	Document& m_document;
	int m_windowNumber;
	std::string m_title;
	EditorState& m_state;
	EditorController& m_controller;
	sf::RenderWindow m_window;
	Toolbar m_toolbar;
	ColorPicker m_colorPicker;
	sf::FloatRect m_fillButtonRect;
	sf::FloatRect m_borderButtonRect;
	CanvasView m_canvasView;
	ShapeListView m_shapeListView;
	TimelineView m_timelineView;
	std::map<CursorType, sf::Cursor> m_cursors;
	CursorType m_currentCursor = CursorType::Arrow;
	MouseCapture m_mouseCapture = MouseCapture::None;
	bool m_isTogglePressed = false;
	bool m_isShiftPressed = false;
	bool m_isCommandPressed = false;

	void InitToolbar();
	void OpenColorPicker(const std::string& title, sf::FloatRect buttonRect, bool isFill);
	void CancelColorPicker();
	void InitCursors();
	void HandleEvent(const sf::Event& event);
	void TrackMouse();
	void SkipInputEvents();
	void HandleMousePressed(sf::Vector2f point);
	void HandleMouseMoved(sf::Vector2f point);
	void HandleMouseReleased(sf::Vector2f point);
	void HandleKeyPressed(const sf::Event::KeyPressed& key);
	void HandleMouseWheel(sf::Vector2f point, sf::Mouse::Wheel wheel, float delta);
	void ZoomIn();
	void ZoomOut();
	void ResetZoom();
	bool IsMouseOverTimeline() const;
	void UpdateCursor(sf::Vector2f point);
	void Render();
	void NewDocument();
	void NewWindow();
	void UpdateTitle();
	std::string GetTitle() const;
	void GroupShapes();
	void UngroupShape();
	void Undo();
	void Redo();
	void OpenDocument();
	void SaveDocument();
	void RunFileOperation(const std::string& errorMessage, const std::function<void()>& operation);
	static bool IsToggleModifierKey(sf::Keyboard::Key code);
};

#endif // OOD_EDITOR_EDITORWINDOW_H
