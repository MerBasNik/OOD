//
// Created by Вадим Патрушев on 15.10.2026.
//

#include "CanvasView.h"
#include "Canvas.h"
#include "Model/EditorState/SelectionFrame.h"
#include "Model/Rendering/RenderShape.h"

const sf::Color CANVAS_COLOR = sf::Color::White;
const sf::Color CANVAS_BORDER_COLOR(180, 180, 180);
const sf::Color SELECTION_COLOR(30, 120, 230);

CanvasView::CanvasView(Document& document, EditorState& state, const sf::Vector2f origin)
	: m_document(document)
	, m_state(state)
	, m_origin(origin)
{
	m_document.AddObserver(*this);
	m_state.AddObserver(*this);
}

CanvasView::~CanvasView()
{
	m_document.RemoveObserver(*this);
	m_state.RemoveObserver(*this);
}

void CanvasView::Draw(sf::RenderTarget& target) const
{
	DrawBackground(target);
	DrawShapes(target);
	DrawSelection(target);
}

Point CanvasView::ToDocumentPoint(const sf::Vector2f point) const
{
	return { point.x - m_origin.x, point.y - m_origin.y };
}

bool CanvasView::IsChange() const
{
	return m_isChange;
}

void CanvasView::SetIsChange()
{
	m_isChange = true;
}

void CanvasView::SetDontChange()
{
	m_isChange = false;
}

void CanvasView::DocumentUpdate()
{
	SetIsChange();
}

void CanvasView::EditorStateUpdate()
{
	SetIsChange();
}

void CanvasView::DrawBackground(sf::RenderTarget& target) const
{
	const Frame bounds = m_document.GetBounds();
	sf::RectangleShape background(sf::Vector2f(static_cast<float>(bounds.m_width), static_cast<float>(bounds.m_height)));
	background.setPosition(ToTargetPoint({ bounds.m_left, bounds.m_top }));
	background.setFillColor(CANVAS_COLOR);
	background.setOutlineColor(CANVAS_BORDER_COLOR);
	background.setOutlineThickness(1.f);
	target.draw(background);
}

void CanvasView::DrawShapes(sf::RenderTarget& target) const
{
	Canvas canvas(target, m_origin);
	RenderShape renderer(canvas);
	const auto selectedId = m_state.GetSelectedShapeId();
	const auto previewFrame = m_state.GetPreviewFrame();
	for (size_t i = 0; i < m_document.GetShapeCount(); ++i)
	{
		const IShape& shape = m_document.GetShape(i);
		if (previewFrame && m_document.GetShapeId(i) == selectedId)
		{
			const auto preview = shape.Clone();
			preview->SetFrame(*previewFrame);
			preview->Accept(renderer);
		}
		else
		{
			shape.Accept(renderer);
		}
	}
	const IShape* creatingShape = m_state.GetCreatingShape();
	if (creatingShape)
	{
		creatingShape->Accept(renderer);
	}
}

void CanvasView::DrawSelection(sf::RenderTarget& target) const
{
	const auto selectedId = m_state.GetSelectedShapeId();
	if (!selectedId)
	{
		return;
	}
	const IShape* shape = m_document.FindShape(*selectedId);
	if (!shape)
	{
		return;
	}

	const Frame frame = m_state.GetPreviewFrame().value_or(shape->GetFrame());
	sf::RectangleShape border(sf::Vector2f(static_cast<float>(frame.m_width), static_cast<float>(frame.m_height)));
	border.setPosition(ToTargetPoint({ frame.m_left, frame.m_top }));
	border.setFillColor(sf::Color::Transparent);
	border.setOutlineColor(SELECTION_COLOR);
	border.setOutlineThickness(1.f);
	target.draw(border);

	for (const auto handle : SelectionFrame::ALL_HANDLES)
	{
		const Frame handleFrame = SelectionFrame::GetHandleFrame(frame, handle);
		sf::RectangleShape marker(sf::Vector2f(static_cast<float>(handleFrame.m_width), static_cast<float>(handleFrame.m_height)));
		marker.setPosition(ToTargetPoint({ handleFrame.m_left, handleFrame.m_top }));
		marker.setFillColor(sf::Color::White);
		marker.setOutlineColor(SELECTION_COLOR);
		marker.setOutlineThickness(-1.f);
		target.draw(marker);
	}
}

sf::Vector2f CanvasView::ToTargetPoint(const Point point) const
{
	return m_origin + sf::Vector2f(static_cast<float>(point.m_x), static_cast<float>(point.m_y));
}
