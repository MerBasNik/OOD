//
// Created by Вадим Патрушев on 18.10.2026.
//

#include "FrameCalculator.h"
#include <algorithm>

double FrameCalculator::Clamp(const double value, const double lo, const double hi)
{
	return std::max(lo, std::min(value, hi));
}

Frame FrameCalculator::CalculateMovedFrame(const Frame& startFrame, const Point offset, const Frame& bounds)
{
	Frame frame = startFrame;
	frame.m_left = Clamp(frame.m_left + offset.m_x, bounds.m_left, bounds.GetRight() - frame.m_width);
	frame.m_top = Clamp(frame.m_top + offset.m_y, bounds.m_top, bounds.GetBottom() - frame.m_height);
	return frame;
}

Frame FrameCalculator::CalculateResizedFrame(const Frame& startFrame, const ResizeHandle handle, const Point offset,
	const Frame& bounds, const double minSize)
{
	double left = startFrame.m_left;
	double top = startFrame.m_top;
	double right = startFrame.GetRight();
	double bottom = startFrame.GetBottom();

	if (SelectionGeometry::IsLeftMove(handle))
	{
		left = Clamp(left + offset.m_x, bounds.m_left, right - minSize);
	}
	if (SelectionGeometry::IsRightMove(handle))
	{
		right = Clamp(right + offset.m_x, left + minSize, bounds.GetRight());
	}
	if (SelectionGeometry::IsTopMove(handle))
	{
		top = Clamp(top + offset.m_y, bounds.m_top, bottom - minSize);
	}
	if (SelectionGeometry::IsBottomMove(handle))
	{
		bottom = Clamp(bottom + offset.m_y, top + minSize, bounds.GetBottom());
	}
	return { left, top, right - left, bottom - top };
}
