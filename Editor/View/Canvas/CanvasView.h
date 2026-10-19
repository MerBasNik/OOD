//
// Created by Вадим Патрушев on 15.10.2026.
//

#ifndef OOD_EDITOR_CANVASVIEW_H
#define OOD_EDITOR_CANVASVIEW_H

#include "Model/Document/Document.h"
#include "Model/EditorState/EditorState.h"
#include <SFML/Graphics.hpp>

class CanvasView : public IDocumentObserver
	, public IEditorStateObserver
{
public:
	CanvasView(Document& document, EditorState& state, sf::Vector2f origin);
	~CanvasView() override;
	CanvasView(const CanvasView&) = delete;
	CanvasView& operator=(const CanvasView&) = delete;
	void Draw(sf::RenderTarget& target) const;
	Point ToDocumentPoint(sf::Vector2f point) const;
	bool IsChange() const;
	void SetIsChange();
	void SetDontChange();
	void DocumentUpdate() override;
	void EditorStateUpdate() override;

private:
	Document& m_document;
	EditorState& m_state;
	sf::Vector2f m_origin;
	bool m_isChange = true;

	void DrawBackground(sf::RenderTarget& target) const;
	void DrawShapes(sf::RenderTarget& target) const;
	void DrawSelection(sf::RenderTarget& target) const;
	sf::Vector2f ToTargetPoint(Point point) const;
};

#endif // OOD_EDITOR_CANVASVIEW_H
