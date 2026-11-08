//
// Created by Вадим Патрушев on 15.10.2026.
//

#include "CanvasView.h"
#include "Canvas.h"
#include "Model/EditorState/SelectionFrame.h"
#include "Model/Rendering/RenderShape.h"
#include <cmath>

const sf::Color CANVAS_COLOR = sf::Color::White;
const sf::Color CANVAS_BORDER_COLOR(180, 180, 180);
const sf::Color SELECTION_COLOR(30, 120, 230);
const double MIN_ZOOM = 0.25;
const double MAX_ZOOM = 8;

CanvasView::CanvasView(Document& document, EditorState& state, const sf::FloatRect& rect)
	: m_document(document)
	, m_state(state)
	, m_rect(rect)
	, m_viewport({ document.GetBounds().m_width, document.GetBounds().m_height }, { rect.size.x, rect.size.y },
		  MIN_ZOOM, MAX_ZOOM)
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
	const sf::View view = target.getView();
	target.setView(GetCanvasView(target));
	DrawBackground(target);
	DrawShapes(target);
	DrawSelection(target);
	target.setView(view);
}

Point CanvasView::ToDocumentPoint(const sf::Vector2f point) const
{
	return m_viewport.ToContent({ point.x - m_rect.position.x, point.y - m_rect.position.y });
}

bool CanvasView::IsContains(const sf::Vector2f point) const
{
	return m_rect.contains(point);
}

void CanvasView::ZoomAt(const sf::Vector2f point, const float delta)
{
	m_viewport.ZoomAt(std::pow(Viewport::ZOOM_STEP, delta), { point.x - m_rect.position.x, point.y - m_rect.position.y });
	SetIsChange();
}

void CanvasView::Scroll(const sf::Vector2f delta)
{
	m_viewport.Scroll({ delta.x, delta.y });
	SetIsChange();
}

void CanvasView::ZoomIn()
{
	m_viewport.ZoomIn();
	SetIsChange();
}

void CanvasView::ZoomOut()
{
	m_viewport.ZoomOut();
	SetIsChange();
}

void CanvasView::ResetZoom()
{
	m_viewport.ResetZoom();
	SetIsChange();
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
	Canvas canvas(target, m_rect.position);
	RenderShape renderer(canvas, m_state.GetCurrentTime());
	const auto previewFillColor = m_state.GetPreviewFillColor();
	const auto previewBorderColor = m_state.GetPreviewBorderColor();
	const bool hasColorPreview = previewFillColor || previewBorderColor;
	for (size_t i = 0; i < m_document.GetShapeCount(); ++i)
	{
		const IShape& shape = m_document.GetShape(i);
		const int id = m_document.GetShapeId(i);
		if (!IsShapeVisible(id))
		{
			continue;
		}
		const auto previewFrame = m_state.GetPreviewFrame(id);
		const auto previewTimeInterval = m_state.GetPreviewTimeInterval(id);
		const bool hasShapeColorPreview = hasColorPreview && m_state.IsShapeSelected(id) && !shape.GetGroup();
		if (previewFrame || previewTimeInterval || hasShapeColorPreview)
		{
			const auto preview = shape.Clone();
			preview->SetFrame(previewFrame ? *previewFrame : shape.GetFrame());
			if (previewTimeInterval)
			{
				preview->SetTimeInterval(*previewTimeInterval);
			}
			if (hasShapeColorPreview)
			{
				preview->SetFillColor(previewFillColor ? *previewFillColor : shape.GetFillColor());
				preview->SetBorderColor(previewBorderColor ? *previewBorderColor : shape.GetBorderColor());
			}
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
	for (const int id : m_state.GetSelectedShapeIds())
	{
		const IShape* shape = m_document.FindShape(id);
		if (!shape || !IsShapeVisible(id))
		{
			continue;
		}
		const auto previewFrame = m_state.GetPreviewFrame(id);
		DrawSelectionFrame(target, previewFrame ? *previewFrame : shape->GetFrame());
	}
}

bool CanvasView::IsShapeVisible(const int id) const
{
	const auto preview = m_state.GetPreviewTimeInterval(id);
	const TimeInterval interval = preview ? *preview : m_document.FindShape(id)->GetTimeInterval();
	return interval.IsContains(m_state.GetCurrentTime());
}

void CanvasView::DrawSelectionFrame(sf::RenderTarget& target, const Frame& frame) const
{
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
	return m_rect.position + sf::Vector2f(static_cast<float>(point.m_x), static_cast<float>(point.m_y));
}

sf::View CanvasView::GetCanvasView(const sf::RenderTarget& target) const
{
	const sf::Vector2f targetSize(target.getSize());
	const Point offset = m_viewport.GetOffset();
	const auto zoom = static_cast<float>(m_viewport.GetZoom());
	const sf::Vector2f position = m_rect.position + sf::Vector2f(static_cast<float>(offset.m_x), static_cast<float>(offset.m_y));
	sf::View view(sf::FloatRect(position, m_rect.size / zoom));
	view.setViewport(sf::FloatRect(m_rect.position.componentWiseDiv(targetSize), m_rect.size.componentWiseDiv(targetSize)));
	return view;
}
