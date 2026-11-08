//
// Created by Вадим Патрушев on 08.11.2026.
//

#ifndef OOD_EDITOR_TIMELINEVIEW_H
#define OOD_EDITOR_TIMELINEVIEW_H

#include "Controller/Enums/CursorType.h"
#include "Controller/TimelineController.h"
#include "Model/Document/Document.h"
#include "Model/EditorState/EditorState.h"
#include "Model/Shapes/ShapeType.h"
#include "TimelineHitResult.h"
#include "View/Viewport/Viewport.h"
#include <SFML/Graphics.hpp>
#include <map>
#include <optional>
#include <vector>

class TimelineView : public IDocumentObserver
	, public IEditorStateObserver
{
public:
	static constexpr float HEIGHT = 200.f;

	TimelineView(Document& document, EditorState& state, TimelineController& controller, const sf::FloatRect& rect);
	~TimelineView() override;
	void Draw(sf::RenderTarget& target) const;
	bool IsContains(sf::Vector2f point) const;
	void OnMouseDown(sf::Vector2f point, bool isToggle);
	void OnMouseMove(sf::Vector2f point);
	void OnMouseUp(sf::Vector2f point);
	void OnMouseWheel(sf::Vector2f point, float delta);
	void ZoomAt(sf::Vector2f point, float delta);
	void ScrollTime(float delta);
	void ZoomIn();
	void ZoomOut();
	void ResetZoom();
	void CancelDrag();
	CursorType GetCursorType(sf::Vector2f point) const;
	bool IsChange() const;
	void SetIsChange();
	void SetDontChange();
	void DocumentUpdate() override;
	void EditorStateUpdate() override;

private:
	Document& m_document;
	EditorState& m_state;
	TimelineController& m_controller;
	sf::FloatRect m_rect;
	sf::Font m_font;
	std::map<ShapeType, sf::Texture> m_icons;
	float m_scroll = 0;
	Viewport m_timeViewport;
	std::vector<int> m_shownSelectedIds;
	bool m_isChange = true;

	TimelineHitResult HitTest(sf::Vector2f point) const;
	std::optional<size_t> GetRowAt(sf::Vector2f point) const;
	size_t GetSlotAt(sf::Vector2f point) const;
	TimeInterval GetShownTimeInterval(size_t row) const;
	void DrawTimeScale(sf::RenderTarget& target) const;
	void DrawRow(sf::RenderTarget& target, size_t row) const;
	void DrawRowSelection(sf::RenderTarget& target, float top) const;
	void DrawRowLabel(sf::RenderTarget& target, const IShape& shape, int id, float top) const;
	void DrawTimeBar(sf::RenderTarget& target, size_t row, const IShape& shape, bool isSelected, float top) const;
	void DrawRowSeparator(sf::RenderTarget& target, float top) const;
	void DrawDropIndicator(sf::RenderTarget& target, size_t slot) const;
	void DrawCurrentTimeLine(sf::RenderTarget& target) const;
	sf::View GetRowsView(const sf::RenderTarget& target) const;
	float GetRowsTop() const;
	float GetRowsHeight() const;
	float GetTrackLeft() const;
	float GetTrackAreaLeft() const;
	float GetTrackWidth() const;
	double GetTimeScaleStep() const;
	float GetPositionByTime(double time) const;
	double GetTimeByPosition(float x) const;
	size_t RowToIndex(size_t row) const;
	void SetScroll(float scroll);
	void ScrollToSelectedShape();
};

#endif // OOD_EDITOR_TIMELINEVIEW_H
