//
// Created by Вадим Патрушев on 15.10.2026.
//

#ifndef OOD_EDITOR_EDITORWINDOW_H
#define OOD_EDITOR_EDITORWINDOW_H

#include "CanvasView.h"
#include "Controller/EditorController.h"
#include "Toolbar.h"
#include <SFML/Graphics.hpp>
#include <map>

class EditorWindow
{
public:
	EditorWindow(Document& document, EditorState& state, EditorController& controller);
	void Run();

private:
	EditorState& m_state;
	EditorController& m_controller;
	sf::RenderWindow m_window;
	Toolbar m_toolbar;
	CanvasView m_canvasView;
	std::map<CursorType, sf::Cursor> m_cursors;
	CursorType m_currentCursor = CursorType::Arrow;
	bool m_isMousePressed = false;

	void InitToolbar();
	void InitCursors();
	void HandleEvent(const sf::Event& event);
	void HandleKeyPressed(const sf::Event::KeyPressed& key);
	void UpdateCursor(sf::Vector2f point);
	void Render();

	void NewDocument();
	void OpenDocument();
	void SaveDocument();

	Point ToDocumentPoint(sf::Vector2i pixel) const;
};

#endif // OOD_EDITOR_EDITORWINDOW_H
