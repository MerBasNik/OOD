//
// Created by Вадим Патрушев on 26.10.2026.
//

#ifndef OOD_EDITOR_SHAPELISTVIEW_H
#define OOD_EDITOR_SHAPELISTVIEW_H

#include "Controller/ShapeListController.h"
#include "Model/Document/Document.h"
#include "Model/EditorState/EditorState.h"
#include <SFML/Graphics.hpp>
#include <map>
#include <optional>

class ShapeListView : public IDocumentObserver
	, public IEditorStateObserver
{
public:
	static constexpr float WIDTH = 220.f;

	ShapeListView(Document& document, EditorState& state, ShapeListController& controller, const sf::FloatRect& rect);
	~ShapeListView() override;
	ShapeListView(const ShapeListView&) = delete;
	ShapeListView& operator=(const ShapeListView&) = delete;
	void Draw(sf::RenderTarget& target) const;
	bool IsContains(sf::Vector2f point) const;
	void OnMouseDown(sf::Vector2f point);
	void OnMouseMove(sf::Vector2f point);
	void OnMouseUp(sf::Vector2f point);
	void OnMouseWheel(sf::Vector2f point, float delta);
	void CancelDrag();
	bool IsChange() const;
	void SetIsChange();
	void SetDontChange();
	void DocumentUpdate() override;
	void EditorStateUpdate() override;

private:
	Document& m_document;
	EditorState& m_state;
	ShapeListController& m_controller;
	sf::FloatRect m_rect;
	sf::Font m_font;
	std::map<ShapeType, sf::Texture> m_icons;
	float m_scroll = 0;
	std::optional<int> m_shownSelectedId;
	bool m_isChange = true;

	std::optional<size_t> GetRowAt(sf::Vector2f point) const;
	size_t GetSlotAt(sf::Vector2f point) const;
	void DrawRow(sf::RenderTarget& target, size_t row) const;
	void DrawDropIndicator(sf::RenderTarget& target, size_t slot) const;
	void DrawScrollbar(sf::RenderTarget& target) const;
	sf::View GetListView(const sf::RenderTarget& target) const;
	float GetContentHeight() const;
	float ToListY(sf::Vector2f point) const;
	void SetScroll(float scroll);
	void ScrollToSelectedShape();
};

#endif // OOD_EDITOR_SHAPELISTVIEW_H
