//
// Created by Вадим Патрушев on 26.10.2026.
//

#include "ShapeListView.h"
#include "View/ShapeIcons.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>

const float ROW_HEIGHT = 28.f;
const float ROW_PADDING = 8.f;
const float ICON_SIZE = 18.f;
const float DROP_INDICATOR_HEIGHT = 2.f;
const float SCROLLBAR_WIDTH = 4.f;
const float SCROLLBAR_MARGIN = 2.f;
const unsigned FONT_SIZE = 13;
const std::string FONT_PATH = "/System/Library/Fonts/Supplemental/Arial.ttf";
const sf::Color PANEL_COLOR(245, 245, 245);
const sf::Color PANEL_BORDER_COLOR(180, 180, 180);
const sf::Color ROW_SEPARATOR_COLOR(225, 225, 225);
const sf::Color SELECTED_ROW_COLOR(205, 225, 250);
const sf::Color DROP_INDICATOR_COLOR(30, 120, 230);
const sf::Color SCROLLBAR_COLOR(160, 160, 160);
const sf::Color TEXT_COLOR(30, 30, 30);

static std::string GetShapeName(const ShapeType type)
{
	switch (type)
	{
	case ShapeType::Rectangle:
		return "Прямоугольник";
	case ShapeType::Ellipse:
		return "Эллипс";
	case ShapeType::Triangle:
		return "Треугольник";
	}
	return {};
}

ShapeListView::ShapeListView(Document& document, EditorState& state, ShapeListController& controller,
	const sf::FloatRect& rect)
	: m_document(document)
	, m_state(state)
	, m_controller(controller)
	, m_rect(rect)
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

ShapeListView::~ShapeListView()
{
	m_document.RemoveObserver(*this);
	m_state.RemoveObserver(*this);
}

void ShapeListView::Draw(sf::RenderTarget& target) const
{
	sf::RectangleShape background(m_rect.size);
	background.setPosition(m_rect.position);
	background.setFillColor(PANEL_COLOR);
	background.setOutlineColor(PANEL_BORDER_COLOR);
	background.setOutlineThickness(-1.f);
	target.draw(background);

	const sf::View view = target.getView();
	target.setView(GetListView(target));
	for (size_t row = 0; row < m_document.GetShapeCount(); ++row)
	{
		DrawRow(target, row);
	}
	if (const auto dropSlot = m_controller.GetDropSlot())
	{
		DrawDropIndicator(target, *dropSlot);
	}
	target.setView(view);

	DrawScrollbar(target);
}

bool ShapeListView::IsContains(const sf::Vector2f point) const
{
	return m_rect.contains(point);
}

void ShapeListView::OnMouseDown(const sf::Vector2f point)
{
	m_controller.OnMouseDown(GetRowAt(point));
	SetIsChange();
}

void ShapeListView::OnMouseMove(const sf::Vector2f point)
{
	m_controller.OnMouseMove(GetSlotAt(point));
	SetIsChange();
}

void ShapeListView::OnMouseUp(const sf::Vector2f point)
{
	m_controller.OnMouseUp(GetSlotAt(point));
	SetIsChange();
}

void ShapeListView::OnMouseWheel(const sf::Vector2f point, const float delta)
{
	if (IsContains(point))
	{
		SetScroll(m_scroll - delta * ROW_HEIGHT);
	}
}

void ShapeListView::CancelDrag()
{
	m_controller.Cancel();
	SetIsChange();
}

std::optional<size_t> ShapeListView::GetRowAt(const sf::Vector2f point) const
{
	if (!IsContains(point))
	{
		return std::nullopt;
	}
	const auto row = static_cast<size_t>(ToListY(point) / ROW_HEIGHT);
	if (row >= m_document.GetShapeCount())
	{
		return std::nullopt;
	}
	return row;
}

size_t ShapeListView::GetSlotAt(const sf::Vector2f point) const
{
	const float slot = std::round(ToListY(point) / ROW_HEIGHT);
	return static_cast<size_t>(std::clamp(slot, 0.f, static_cast<float>(m_document.GetShapeCount())));
}

bool ShapeListView::IsChange() const
{
	return m_isChange;
}

void ShapeListView::SetIsChange()
{
	m_isChange = true;
}

void ShapeListView::SetDontChange()
{
	m_isChange = false;
}

void ShapeListView::DocumentUpdate()
{
	SetScroll(m_scroll);
	SetIsChange();
}

void ShapeListView::EditorStateUpdate()
{
	if (m_state.GetSelectedShapeId() != m_shownSelectedId)
	{
		m_shownSelectedId = m_state.GetSelectedShapeId();
		ScrollToSelectedShape();
	}
	SetIsChange();
}

void ShapeListView::DrawRow(sf::RenderTarget& target, const size_t row) const
{
	const size_t index = m_document.GetShapeCount() - 1 - row;
	const IShape& shape = m_document.GetShape(index);
	const float top = static_cast<float>(row) * ROW_HEIGHT;

	if (m_document.GetShapeId(index) == m_state.GetSelectedShapeId())
	{
		sf::RectangleShape selection({ m_rect.size.x, ROW_HEIGHT });
		selection.setPosition({ 0.f, top });
		selection.setFillColor(SELECTED_ROW_COLOR);
		target.draw(selection);
	}

	const sf::Texture& iconTexture = m_icons.at(shape.GetType());
	const sf::Vector2f iconSize(iconTexture.getSize());
	sf::Sprite icon(iconTexture);
	icon.setScale(sf::Vector2f(ICON_SIZE, ICON_SIZE).componentWiseDiv(iconSize));
	icon.setPosition({ ROW_PADDING, top + (ROW_HEIGHT - ICON_SIZE) / 2 });
	target.draw(icon);

	const std::string label = GetShapeName(shape.GetType()) + " " + std::to_string(m_document.GetShapeId(index));
	sf::Text text(m_font, sf::String::fromUtf8(label.begin(), label.end()), FONT_SIZE);
	text.setFillColor(TEXT_COLOR);
	const sf::FloatRect textBounds = text.getLocalBounds();
	text.setOrigin({ 0.f, textBounds.position.y + textBounds.size.y / 2 });
	text.setPosition({ 2 * ROW_PADDING + ICON_SIZE, top + ROW_HEIGHT / 2 });
	target.draw(text);

	sf::RectangleShape separator({ m_rect.size.x, 1.f });
	separator.setPosition({ 0.f, top + ROW_HEIGHT - 1.f });
	separator.setFillColor(ROW_SEPARATOR_COLOR);
	target.draw(separator);
}

void ShapeListView::DrawDropIndicator(sf::RenderTarget& target, const size_t slot) const
{
	const float top = static_cast<float>(slot) * ROW_HEIGHT - DROP_INDICATOR_HEIGHT / 2;
	sf::RectangleShape indicator({ m_rect.size.x, DROP_INDICATOR_HEIGHT });
	indicator.setPosition({ 0.f, std::clamp(top, m_scroll, m_scroll + m_rect.size.y - DROP_INDICATOR_HEIGHT) });
	indicator.setFillColor(DROP_INDICATOR_COLOR);
	target.draw(indicator);
}

void ShapeListView::DrawScrollbar(sf::RenderTarget& target) const
{
	const float contentHeight = GetContentHeight();
	if (contentHeight <= m_rect.size.y)
	{
		return;
	}
	const float thumbHeight = m_rect.size.y * m_rect.size.y / contentHeight;
	const float thumbTop = m_rect.position.y + m_scroll / contentHeight * m_rect.size.y;
	sf::RectangleShape thumb({ SCROLLBAR_WIDTH, thumbHeight });
	thumb.setPosition({ m_rect.position.x + m_rect.size.x - SCROLLBAR_WIDTH - SCROLLBAR_MARGIN, thumbTop });
	thumb.setFillColor(SCROLLBAR_COLOR);
	target.draw(thumb);
}

sf::View ShapeListView::GetListView(const sf::RenderTarget& target) const
{
	const sf::Vector2f targetSize(target.getSize());
	sf::View view(sf::FloatRect({ 0.f, m_scroll }, m_rect.size));
	view.setViewport(sf::FloatRect(m_rect.position.componentWiseDiv(targetSize), m_rect.size.componentWiseDiv(targetSize)));
	return view;
}

float ShapeListView::GetContentHeight() const
{
	return static_cast<float>(m_document.GetShapeCount()) * ROW_HEIGHT;
}

float ShapeListView::ToListY(const sf::Vector2f point) const
{
	return point.y - m_rect.position.y + m_scroll;
}

void ShapeListView::SetScroll(const float scroll)
{
	const float maxScroll = std::max(0.f, GetContentHeight() - m_rect.size.y);
	const float newScroll = std::clamp(scroll, 0.f, maxScroll);
	if (newScroll == m_scroll)
	{
		return;
	}
	m_scroll = newScroll;
	SetIsChange();
}

void ShapeListView::ScrollToSelectedShape()
{
	const auto selectedId = m_state.GetSelectedShapeId();
	const auto index = selectedId ? m_document.FindShapeIndex(*selectedId) : std::nullopt;
	if (!index)
	{
		return;
	}
	const float top = static_cast<float>(m_document.GetShapeCount() - 1 - *index) * ROW_HEIGHT;
	if (top < m_scroll)
	{
		SetScroll(top);
	}
	else if (top + ROW_HEIGHT > m_scroll + m_rect.size.y)
	{
		SetScroll(top + ROW_HEIGHT - m_rect.size.y);
	}
}
