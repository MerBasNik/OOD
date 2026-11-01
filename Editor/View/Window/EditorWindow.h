//
// Created by Вадим Патрушев on 15.10.2026.
//

#ifndef OOD_EDITOR_EDITORWINDOW_H
#define OOD_EDITOR_EDITORWINDOW_H

#include "ColorPicker.h"
#include "Controller/EditorController.h"
#include "Toolbar.h"
#include "Enums/MouseCapture.h"
#include "View/Canvas/CanvasView.h"
#include "View/ShapeList/ShapeListView.h"
#include <SFML/Graphics.hpp>
#include <functional>
#include <map>
#include <string>

class EditorWindow
{
public:
	EditorWindow(Document& document, EditorState& state, EditorController& controller,
		ShapeListController& shapeListController);
	void Run();

private:
	Document& m_document;
	EditorState& m_state;
	EditorController& m_controller;
	sf::RenderWindow m_window;
	Toolbar m_toolbar;
	ColorPicker m_colorPicker;
	sf::FloatRect m_fillButtonRect;
	sf::FloatRect m_borderButtonRect;
	CanvasView m_canvasView;
	ShapeListView m_shapeListView;
	std::map<CursorType, sf::Cursor> m_cursors;
	CursorType m_currentCursor = CursorType::Arrow;
	MouseCapture m_mouseCapture = MouseCapture::None;
	bool m_isTogglePressed = false;

	void InitToolbar();
	void OpenColorPicker(const std::string& title, sf::FloatRect buttonRect, bool isFill);
	void CancelColorPicker();
	void InitCursors();
	void HandleEvent(const sf::Event& event);
	void HandleMousePressed(sf::Vector2f point);
	void HandleMouseMoved(sf::Vector2f point);
	void HandleMouseReleased(sf::Vector2f point);
	void HandleKeyPressed(const sf::Event::KeyPressed& key);
	void UpdateCursor(sf::Vector2f point);
	void Render();
	void NewDocument();
	void Undo();
	void Redo();
	void OpenDocument();
	void SaveDocument();
	void RunFileOperation(const std::string& errorMessage, const std::function<void()>& operation);
	static bool IsToggleModifierKey(sf::Keyboard::Key code);
};

#endif // OOD_EDITOR_EDITORWINDOW_H
