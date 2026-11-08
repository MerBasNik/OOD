//
// Created by Вадим Патрушев on 08.11.2026.
//

#include "TimelineView.h"
#include "View/Canvas/Canvas.h"
#include "View/ShapeIcons.h"
#include "View/ShapeList/ShapeListItemVisitor.h"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

const float RULER_HEIGHT = 24.f;
const float ROW_HEIGHT = 24.f;
const float LABEL_WIDTH = 160.f;
const float TRACK_PADDING = 12.f;
const float ROW_PADDING = 8.f;
const float BAR_MARGIN = 4.f;
const float EDGE_GRAB_WIDTH = 5.f;
const float ICON_SIZE = 16.f;
const float TICK_HEIGHT = 6.f;
const float DROP_INDICATOR_HEIGHT = 2.f;
const float MIN_TICK_SPACING = 60.f;
const double MAX_TIME_ZOOM = 50;
const float CURRENT_TIME_LINE_WIDTH = 2.f;
const unsigned FONT_SIZE = 12;
const std::string FONT_PATH = "/System/Library/Fonts/Supplemental/Arial.ttf";
const sf::Color PANEL_COLOR(245, 245, 245);
const sf::Color PANEL_BORDER_COLOR(180, 180, 180);
const sf::Color RULER_COLOR(230, 230, 230);
const sf::Color ROW_SEPARATOR_COLOR(225, 225, 225);
const sf::Color SELECTED_ROW_COLOR(205, 225, 250);
const sf::Color BAR_BORDER_COLOR(90, 90, 90);
const sf::Color SELECTION_COLOR(30, 120, 230);
const sf::Color CURRENT_TIME_COLOR(220, 40, 40);
const sf::Color TEXT_COLOR(30, 30, 30);

TimelineView::TimelineView(Document& document, EditorState& state, TimelineController& controller,
	const sf::FloatRect& rect)
	: m_document(document)
	, m_state(state)
	, m_controller(controller)
	, m_rect(rect)
	, m_timeViewport({ rect.size.x - LABEL_WIDTH - 2 * TRACK_PADDING, 1 }, { rect.size.x - LABEL_WIDTH - 2 * TRACK_PADDING, 1 },
		  1, MAX_TIME_ZOOM)
{
	if (!m_font.openFromFile(FONT_PATH))
	{
		std::cerr << "Ошибка при загрузке шрифта" << std::endl;
	}
	for (const auto type : { ShapeType::Rectangle, ShapeType::Ellipse, ShapeType::Triangle })
	{
		m_icons.emplace(type, ShapeIcons::Load(type));
	}
	m_document.AddObserver(*this);
	m_state.AddObserver(*this);
}

TimelineView::~TimelineView()
{
	m_document.RemoveObserver(*this);
	m_state.RemoveObserver(*this);
}

void TimelineView::Draw(sf::RenderTarget& target) const
{
	sf::RectangleShape background(m_rect.size);
	background.setPosition(m_rect.position);
	background.setFillColor(PANEL_COLOR);
	background.setOutlineColor(PANEL_BORDER_COLOR);
	background.setOutlineThickness(-1.f);
	target.draw(background);

	const sf::View view = target.getView();
	target.setView(GetRowsView(target));
	for (size_t row = 0; row < m_document.GetShapeCount(); ++row)
	{
		DrawRow(target, row);
	}
	if (const auto dropSlot = m_controller.GetDropSlot())
	{
		DrawDropIndicator(target, *dropSlot);
	}
	target.setView(view);

	sf::RectangleShape divider({ 1.f, m_rect.size.y });
	divider.setPosition({ m_rect.position.x + LABEL_WIDTH, m_rect.position.y });
	divider.setFillColor(PANEL_BORDER_COLOR);
	target.draw(divider);

	DrawTimeScale(target);
	DrawCurrentTimeLine(target);
}

bool TimelineView::IsContains(const sf::Vector2f point) const
{
	return m_rect.contains(point);
}

void TimelineView::OnMouseDown(const sf::Vector2f point, const bool isToggle)
{
	const auto [mode, row] = HitTest(point);
	m_controller.OnMouseDown(row, GetTimeByPosition(point.x), mode, isToggle);
	SetIsChange();
}

void TimelineView::OnMouseMove(const sf::Vector2f point)
{
	m_controller.OnMouseMove(GetSlotAt(point), GetTimeByPosition(point.x));
	SetIsChange();
}

void TimelineView::OnMouseUp(const sf::Vector2f point)
{
	m_controller.OnMouseUp(GetSlotAt(point), GetTimeByPosition(point.x));
	SetIsChange();
}

void TimelineView::OnMouseWheel(const sf::Vector2f point, const float delta)
{
	if (IsContains(point))
	{
		SetScroll(m_scroll - delta * ROW_HEIGHT);
	}
}

void TimelineView::ZoomAt(const sf::Vector2f point, const float delta)
{
	m_timeViewport.ZoomAt(std::pow(Viewport::ZOOM_STEP, delta), { point.x - GetTrackLeft(), 0 });
	SetIsChange();
}

void TimelineView::ScrollTime(const float delta)
{
	m_timeViewport.Scroll({ delta, 0 });
	SetIsChange();
}

void TimelineView::ZoomIn()
{
	m_timeViewport.ZoomIn();
	SetIsChange();
}

void TimelineView::ZoomOut()
{
	m_timeViewport.ZoomOut();
	SetIsChange();
}

void TimelineView::ResetZoom()
{
	m_timeViewport.ResetZoom();
	SetIsChange();
}

void TimelineView::CancelDrag()
{
	m_controller.Cancel();
	SetIsChange();
}

CursorType TimelineView::GetCursorType(const sf::Vector2f point) const
{
	TimelineDragMode mode = m_controller.GetDragMode();
	if (mode == TimelineDragMode::None)
	{
		mode = HitTest(point).m_mode;
	}
	switch (mode)
	{
	case TimelineDragMode::MoveTime:
		return CursorType::Move;
	case TimelineDragMode::ResizeStart:
	case TimelineDragMode::ResizeEnd:
		return CursorType::SizeHorizontal;
	case TimelineDragMode::MoveLayers:
		return CursorType::SizeVertical;
	case TimelineDragMode::None:
	case TimelineDragMode::Scrub:
		break;
	}
	return CursorType::Arrow;
}

bool TimelineView::IsChange() const
{
	return m_isChange;
}

void TimelineView::SetIsChange()
{
	m_isChange = true;
}

void TimelineView::SetDontChange()
{
	m_isChange = false;
}

void TimelineView::DocumentUpdate()
{
	SetScroll(m_scroll);
	SetIsChange();
}

void TimelineView::EditorStateUpdate()
{
	if (m_state.GetSelectedShapeIds() != m_shownSelectedIds)
	{
		m_shownSelectedIds = m_state.GetSelectedShapeIds();
		ScrollToSelectedShape();
	}
	SetIsChange();
}

TimelineHitResult TimelineView::HitTest(const sf::Vector2f point) const
{
	if (!IsContains(point))
	{
		return {};
	}
	if (point.y < GetRowsTop())
	{
		return { TimelineDragMode::Scrub, std::nullopt };
	}
	const auto row = GetRowAt(point);
	if (!row)
	{
		return {};
	}
	if (point.x < m_rect.position.x + LABEL_WIDTH)
	{
		return { TimelineDragMode::MoveLayers, row };
	}

	const TimeInterval interval = GetShownTimeInterval(*row);
	const float left = GetPositionByTime(interval.m_start);
	const float right = GetPositionByTime(interval.m_end);
	if (std::abs(point.x - left) <= EDGE_GRAB_WIDTH)
	{
		return { TimelineDragMode::ResizeStart, row };
	}
	if (std::abs(point.x - right) <= EDGE_GRAB_WIDTH)
	{
		return { TimelineDragMode::ResizeEnd, row };
	}
	if (point.x > left && point.x < right)
	{
		return { TimelineDragMode::MoveTime, row };
	}
	return { TimelineDragMode::None, row };
}

std::optional<size_t> TimelineView::GetRowAt(const sf::Vector2f point) const
{
	if (!IsContains(point) || point.y < GetRowsTop())
	{
		return std::nullopt;
	}
	const auto row = static_cast<size_t>((point.y - GetRowsTop() + m_scroll) / ROW_HEIGHT);
	if (row >= m_document.GetShapeCount())
	{
		return std::nullopt;
	}
	return row;
}

size_t TimelineView::GetSlotAt(const sf::Vector2f point) const
{
	const float slot = std::round((point.y - GetRowsTop() + m_scroll) / ROW_HEIGHT);
	return static_cast<size_t>(std::clamp(slot, 0.f, static_cast<float>(m_document.GetShapeCount())));
}

TimeInterval TimelineView::GetShownTimeInterval(const size_t row) const
{
	const size_t index = RowToIndex(row);
	const auto preview = m_state.GetPreviewTimeInterval(m_document.GetShapeId(index));
	return preview ? *preview : m_document.GetShape(index).GetTimeInterval();
}

void TimelineView::DrawTimeScale(sf::RenderTarget& target) const
{
	const float left = m_rect.position.x + LABEL_WIDTH;
	sf::RectangleShape ruler({ m_rect.size.x - LABEL_WIDTH, RULER_HEIGHT });
	ruler.setPosition({ left, m_rect.position.y });
	ruler.setFillColor(RULER_COLOR);
	target.draw(ruler);

	const double step = GetTimeScaleStep();
	const double firstTime = std::max(0.0, GetTimeByPosition(GetTrackAreaLeft()));
	const double lastTime = std::min(m_document.GetDuration(), GetTimeByPosition(m_rect.position.x + m_rect.size.x));
	const auto lastTick = static_cast<int>(std::floor(lastTime / step + 1e-9));
	for (auto tickIndex = static_cast<int>(std::ceil(firstTime / step - 1e-9)); tickIndex <= lastTick; ++tickIndex)
	{
		const double time = tickIndex * step;
		const float x = GetPositionByTime(time);
		sf::RectangleShape tick({ 1.f, TICK_HEIGHT });
		tick.setPosition({ x, m_rect.position.y + RULER_HEIGHT - TICK_HEIGHT });
		tick.setFillColor(BAR_BORDER_COLOR);
		target.draw(tick);

		std::ostringstream label;
		label << std::setprecision(step < 1 ? 1 : 0) << std::fixed << time << "s";
		sf::Text text(m_font, label.str(), FONT_SIZE);
		text.setFillColor(TEXT_COLOR);
		const sf::FloatRect bounds = text.getLocalBounds();
		text.setOrigin({ bounds.position.x + bounds.size.x / 2, 0.f });
		text.setPosition({ x, m_rect.position.y + 3.f });
		target.draw(text);
	}

	sf::RectangleShape border({ m_rect.size.x, 1.f });
	border.setPosition({ m_rect.position.x, GetRowsTop() - 1.f });
	border.setFillColor(PANEL_BORDER_COLOR);
	target.draw(border);
}

void TimelineView::DrawRow(sf::RenderTarget& target, const size_t row) const
{
	const size_t index = RowToIndex(row);
	const IShape& shape = m_document.GetShape(index);
	const int id = m_document.GetShapeId(index);
	const bool isSelected = m_state.IsShapeSelected(id);
	const float top = GetRowsTop() + static_cast<float>(row) * ROW_HEIGHT;

	if (isSelected)
	{
		DrawRowSelection(target, top);
	}
	DrawRowLabel(target, shape, id, top);
	DrawTimeBar(target, row, shape, isSelected, top);
	DrawRowSeparator(target, top);
}

void TimelineView::DrawRowSelection(sf::RenderTarget& target, const float top) const
{
	sf::RectangleShape selection({ m_rect.size.x, ROW_HEIGHT });
	selection.setPosition({ m_rect.position.x, top });
	selection.setFillColor(SELECTED_ROW_COLOR);
	target.draw(selection);
}

void TimelineView::DrawRowLabel(sf::RenderTarget& target, const IShape& shape, const int id, const float top) const
{
	ShapeListItemVisitor item(m_icons);
	shape.Accept(item);
	const sf::Vector2f iconSize(item.GetIcon().getSize());
	sf::Sprite icon(item.GetIcon());
	icon.setScale(sf::Vector2f(ICON_SIZE, ICON_SIZE).componentWiseDiv(iconSize));
	icon.setPosition({ m_rect.position.x + ROW_PADDING, top + (ROW_HEIGHT - ICON_SIZE) / 2 });
	target.draw(icon);

	const std::string label = item.GetName() + " " + std::to_string(id);
	sf::Text text(m_font, sf::String::fromUtf8(label.begin(), label.end()), FONT_SIZE);
	text.setFillColor(TEXT_COLOR);
	const sf::FloatRect textBounds = text.getLocalBounds();
	text.setOrigin({ 0.f, textBounds.position.y + textBounds.size.y / 2 });
	text.setPosition({ m_rect.position.x + 2 * ROW_PADDING + ICON_SIZE, top + ROW_HEIGHT / 2 });
	target.draw(text);
}

void TimelineView::DrawTimeBar(sf::RenderTarget& target, const size_t row, const IShape& shape, const bool isSelected,
	const float top) const
{
	const TimeInterval interval = GetShownTimeInterval(row);
	const float left = std::max(GetPositionByTime(interval.m_start), GetTrackAreaLeft());
	const float right = std::min(GetPositionByTime(interval.m_end), m_rect.position.x + m_rect.size.x);
	if (right <= left)
	{
		return;
	}
	sf::RectangleShape bar({ right - left, ROW_HEIGHT - 2 * BAR_MARGIN });
	bar.setPosition({ left, top + BAR_MARGIN });
	bar.setFillColor(Canvas::ConvertColorToSf(shape.GetFillColor()));
	bar.setOutlineColor(isSelected ? SELECTION_COLOR : BAR_BORDER_COLOR);
	bar.setOutlineThickness(isSelected ? -2.f : -1.f);
	target.draw(bar);
}

void TimelineView::DrawRowSeparator(sf::RenderTarget& target, const float top) const
{
	sf::RectangleShape separator({ m_rect.size.x, 1.f });
	separator.setPosition({ m_rect.position.x, top + ROW_HEIGHT - 1.f });
	separator.setFillColor(ROW_SEPARATOR_COLOR);
	target.draw(separator);
}

void TimelineView::DrawDropIndicator(sf::RenderTarget& target, const size_t slot) const
{
	const float top = GetRowsTop() + static_cast<float>(slot) * ROW_HEIGHT - DROP_INDICATOR_HEIGHT / 2;
	sf::RectangleShape indicator({ m_rect.size.x, DROP_INDICATOR_HEIGHT });
	indicator.setPosition({ m_rect.position.x, top });
	indicator.setFillColor(SELECTION_COLOR);
	target.draw(indicator);
}

void TimelineView::DrawCurrentTimeLine(sf::RenderTarget& target) const
{
	const float x = GetPositionByTime(m_state.GetCurrentTime());
	if (x < GetTrackAreaLeft() || x > m_rect.position.x + m_rect.size.x)
	{
		return;
	}
	sf::RectangleShape line({ CURRENT_TIME_LINE_WIDTH, m_rect.size.y });
	line.setPosition({ x - CURRENT_TIME_LINE_WIDTH / 2, m_rect.position.y });
	line.setFillColor(CURRENT_TIME_COLOR);
	target.draw(line);
}

sf::View TimelineView::GetRowsView(const sf::RenderTarget& target) const
{
	const sf::Vector2f targetSize(target.getSize());
	const sf::Vector2f position(m_rect.position.x, GetRowsTop());
	const sf::Vector2f size(m_rect.size.x, GetRowsHeight());
	sf::View view(sf::FloatRect({ position.x, position.y + m_scroll }, size));
	view.setViewport(sf::FloatRect(position.componentWiseDiv(targetSize), size.componentWiseDiv(targetSize)));
	return view;
}

float TimelineView::GetRowsTop() const
{
	return m_rect.position.y + RULER_HEIGHT;
}

float TimelineView::GetRowsHeight() const
{
	return m_rect.size.y - RULER_HEIGHT;
}

float TimelineView::GetTrackLeft() const
{
	return m_rect.position.x + LABEL_WIDTH + TRACK_PADDING;
}

float TimelineView::GetTrackAreaLeft() const
{
	return m_rect.position.x + LABEL_WIDTH;
}

float TimelineView::GetTrackWidth() const
{
	return m_rect.size.x - LABEL_WIDTH - 2 * TRACK_PADDING;
}

double TimelineView::GetTimeScaleStep() const
{
	const double pixelsPerSecond = GetTrackWidth() / m_document.GetDuration() * m_timeViewport.GetZoom();
	for (const double step : { 0.1, 0.2, 0.5, 1.0, 2.0, 5.0 })
	{
		if (step * pixelsPerSecond >= MIN_TICK_SPACING)
		{
			return step;
		}
	}
	return 10;
}

float TimelineView::GetPositionByTime(const double time) const
{
	const double trackX = time / m_document.GetDuration() * GetTrackWidth();
	return GetTrackLeft() + static_cast<float>(m_timeViewport.ToView({ trackX, 0 }).m_x);
}

double TimelineView::GetTimeByPosition(const float x) const
{
	const double trackX = m_timeViewport.ToContent({ x - GetTrackLeft(), 0 }).m_x;
	return trackX / GetTrackWidth() * m_document.GetDuration();
}

size_t TimelineView::RowToIndex(const size_t row) const
{
	return m_document.GetShapeCount() - 1 - row;
}

void TimelineView::SetScroll(const float scroll)
{
	const float contentHeight = static_cast<float>(m_document.GetShapeCount()) * ROW_HEIGHT;
	const float maxScroll = std::max(0.f, contentHeight - GetRowsHeight());
	const float newScroll = std::clamp(scroll, 0.f, maxScroll);
	if (newScroll == m_scroll)
	{
		return;
	}
	m_scroll = newScroll;
	SetIsChange();
}

void TimelineView::ScrollToSelectedShape()
{
	const auto& selectedIds = m_state.GetSelectedShapeIds();
	const auto index = selectedIds.empty() ? std::nullopt : m_document.FindShapeIndex(selectedIds.back());
	if (!index)
	{
		return;
	}
	const float top = static_cast<float>(m_document.GetShapeCount() - 1 - *index) * ROW_HEIGHT;
	if (top < m_scroll)
	{
		SetScroll(top);
	}
	else if (top + ROW_HEIGHT > m_scroll + GetRowsHeight())
	{
		SetScroll(top + ROW_HEIGHT - GetRowsHeight());
	}
}
