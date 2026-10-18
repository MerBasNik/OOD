//
// Created by Вадим Патрушев on 15.10.2026.
//

#include "SelectionGeometry.h"

Point SelectionGeometry::GetHandleCenter(const Frame& selection, const ResizeHandle handle)
{
	const double left = selection.m_left;
	const double right = selection.GetRight();
	const double top = selection.m_top;
	const double bottom = selection.GetBottom();
	const Point center = selection.GetCenter();

	switch (handle)
	{
	case ResizeHandle::TopLeft:
		return { left, top };
	case ResizeHandle::Top:
		return { center.m_x, top };
	case ResizeHandle::TopRight:
		return { right, top };
	case ResizeHandle::Right:
		return { right, center.m_y };
	case ResizeHandle::BottomRight:
		return { right, bottom };
	case ResizeHandle::Bottom:
		return { center.m_x, bottom };
	case ResizeHandle::BottomLeft:
		return { left, bottom };
	case ResizeHandle::Left:
		return { left, center.m_y };
	}
	return center;
}

Frame SelectionGeometry::GetHandleFrame(const Frame& selection, const ResizeHandle handle)
{
	const Point center = GetHandleCenter(selection, handle);
	return { center.m_x - HANDLE_SIZE / 2, center.m_y - HANDLE_SIZE / 2, HANDLE_SIZE, HANDLE_SIZE };
}

std::optional<ResizeHandle> SelectionGeometry::HitTestHandle(const Frame& selection, const Point point)
{
	for (const auto handle : ALL_HANDLES)
	{
		if (GetHandleFrame(selection, handle).IsContains(point))
		{
			return handle;
		}
	}
	return std::nullopt;
}

bool SelectionGeometry::HitTestBorder(const Frame& selection, const Point point)
{
	const Frame outer{
		selection.m_left - BORDER_SELECT_SIZE,
		selection.m_top - BORDER_SELECT_SIZE,
		selection.m_width + 2 * BORDER_SELECT_SIZE,
		selection.m_height + 2 * BORDER_SELECT_SIZE,
	};
	const Frame inner{
		selection.m_left + BORDER_SELECT_SIZE,
		selection.m_top + BORDER_SELECT_SIZE,
		selection.m_width - 2 * BORDER_SELECT_SIZE,
		selection.m_height - 2 * BORDER_SELECT_SIZE,
	};
	const bool insideInner = inner.m_width > 0 && inner.m_height > 0 && inner.IsContains(point);
	return outer.IsContains(point) && !insideInner;
}

bool SelectionGeometry::IsLeftMove(const ResizeHandle handle)
{
	return handle == ResizeHandle::TopLeft || handle == ResizeHandle::Left || handle == ResizeHandle::BottomLeft;
}

bool SelectionGeometry::IsRightMove(const ResizeHandle handle)
{
	return handle == ResizeHandle::TopRight || handle == ResizeHandle::Right || handle == ResizeHandle::BottomRight;
}

bool SelectionGeometry::IsTopMove(const ResizeHandle handle)
{
	return handle == ResizeHandle::TopLeft || handle == ResizeHandle::Top || handle == ResizeHandle::TopRight;
}

bool SelectionGeometry::IsBottomMove(const ResizeHandle handle)
{
	return handle == ResizeHandle::BottomLeft || handle == ResizeHandle::Bottom || handle == ResizeHandle::BottomRight;
}
