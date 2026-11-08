//
// Created by Вадим Патрушев on 15.10.2026.
//

#ifndef OOD_EDITOR_CANVASVIEW_H
#define OOD_EDITOR_CANVASVIEW_H

#include "Model/Document/Document.h"
#include "Model/EditorState/EditorState.h"
#include "View/Viewport/Viewport.h"
#include <SFML/Graphics.hpp>

class CanvasView : public IDocumentObserver
	, public IEditorStateObserver
{
public:
	CanvasView(Document& document, EditorState& state, const sf::FloatRect& rect);
	~CanvasView() override;
	CanvasView(const CanvasView&) = delete;
	CanvasView& operator=(const CanvasView&) = delete;
	void Draw(sf::RenderTarget& target) const;
	Point ToDocumentPoint(sf::Vector2f point) const;
	bool IsContains(sf::Vector2f point) const;
	void ZoomAt(sf::Vector2f point, float delta);
	void Scroll(sf::Vector2f delta);
	void ZoomIn();
	void ZoomOut();
	void ResetZoom();
	bool IsChange() const;
	void SetIsChange();
	void SetDontChange();
	void DocumentUpdate() override;
	void EditorStateUpdate() override;

private:
	Document& m_document;
	EditorState& m_state;
	sf::FloatRect m_rect;
	Viewport m_viewport;
	bool m_isChange = true;

	void DrawBackground(sf::RenderTarget& target) const;
	void DrawShapes(sf::RenderTarget& target) const;
	void DrawSelection(sf::RenderTarget& target) const;
	bool IsShapeVisible(int id) const;
	void DrawSelectionFrame(sf::RenderTarget& target, const Frame& frame) const;
	sf::Vector2f ToTargetPoint(Point point) const;
	sf::View GetCanvasView(const sf::RenderTarget& target) const;
};

#endif // OOD_EDITOR_CANVASVIEW_H
