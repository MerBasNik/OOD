//
// Created by Вадим Патрушев on 09.11.2026.
//

#include "Viewport.h"
#include <algorithm>
#include <stdexcept>

Viewport::Viewport(const Point contentSize, const Point viewSize, const double minZoom, const double maxZoom)
	: m_contentSize(contentSize)
	, m_viewSize(viewSize)
	, m_minZoom(minZoom)
	, m_maxZoom(maxZoom)
{
	if (minZoom <= 0 || minZoom > 1 || maxZoom < 1)
	{
		throw std::invalid_argument("Недопустимые границы масштаба");
	}
	ClampOffset();
}

double Viewport::GetZoom() const
{
	return m_zoom;
}

Point Viewport::GetOffset() const
{
	return m_offset;
}

Point Viewport::ToContent(const Point viewPoint) const
{
	return { m_offset.m_x + viewPoint.m_x / m_zoom, m_offset.m_y + viewPoint.m_y / m_zoom };
}

Point Viewport::ToView(const Point contentPoint) const
{
	return { (contentPoint.m_x - m_offset.m_x) * m_zoom, (contentPoint.m_y - m_offset.m_y) * m_zoom };
}

void Viewport::ZoomAt(const double factor, const Point viewPoint)
{
	const Point contentPoint = ToContent(viewPoint);
	m_zoom = std::clamp(m_zoom * factor, m_minZoom, m_maxZoom);
	m_offset = { contentPoint.m_x - viewPoint.m_x / m_zoom, contentPoint.m_y - viewPoint.m_y / m_zoom };
	ClampOffset();
}

void Viewport::ZoomIn()
{
	ZoomAt(ZOOM_STEP, GetViewCenter());
}

void Viewport::ZoomOut()
{
	ZoomAt(1 / ZOOM_STEP, GetViewCenter());
}

void Viewport::ResetZoom()
{
	m_zoom = 1;
	m_offset = { 0, 0 };
	ClampOffset();
}

void Viewport::Scroll(const Point viewDelta)
{
	m_offset = { m_offset.m_x + viewDelta.m_x / m_zoom, m_offset.m_y + viewDelta.m_y / m_zoom };
	ClampOffset();
}

Point Viewport::GetViewCenter() const
{
	return { m_viewSize.m_x / 2, m_viewSize.m_y / 2 };
}

void Viewport::ClampOffset()
{
	m_offset = {
		ClampAxis(m_offset.m_x, m_contentSize.m_x, m_viewSize.m_x / m_zoom),
		ClampAxis(m_offset.m_y, m_contentSize.m_y, m_viewSize.m_y / m_zoom),
	};
}

double Viewport::ClampAxis(const double offset, const double contentSize, const double visibleSize)
{
	if (visibleSize >= contentSize)
	{
		return (contentSize - visibleSize) / 2;
	}
	return std::clamp(offset, 0.0, contentSize - visibleSize);
}
