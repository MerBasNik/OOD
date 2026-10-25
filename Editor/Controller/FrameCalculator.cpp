//
// Created by Вадим Патрушев on 18.10.2026.
//

#include "FrameCalculator.h"
#include "Model/EditorState/SelectionFrame.h"
#include <algorithm>

Frame FrameCalculator::CalculateMovedFrame(const Frame& startFrame, const Point offset, const Frame& bounds)
{
	Frame frame = startFrame;
	frame.m_left = std::clamp(frame.m_left + offset.m_x, bounds.m_left, bounds.GetRight() - frame.m_width);
	frame.m_top = std::clamp(frame.m_top + offset.m_y, bounds.m_top, bounds.GetBottom() - frame.m_height);
	return frame;
}

Frame FrameCalculator::CalculateResizedFrame(const Frame& startFrame, const ResizeHandle handle, const Point offset,
	const Frame& bounds, const double minSize)
{
	double left = startFrame.m_left;
	double top = startFrame.m_top;
	double right = startFrame.GetRight();
	double bottom = startFrame.GetBottom();

	if (SelectionFrame::IsLeftMove(handle))
	{
		left = std::clamp(left + offset.m_x, bounds.m_left, right - minSize);
	}
	if (SelectionFrame::IsRightMove(handle))
	{
		right = std::clamp(right + offset.m_x, left + minSize, bounds.GetRight());
	}
	if (SelectionFrame::IsTopMove(handle))
	{
		top = std::clamp(top + offset.m_y, bounds.m_top, bottom - minSize);
	}
	if (SelectionFrame::IsBottomMove(handle))
	{
		bottom = std::clamp(bottom + offset.m_y, top + minSize, bounds.GetBottom());
	}
	return { left, top, right - left, bottom - top };
}

Frame FrameCalculator::CalculateCreatedFrame(const Point startPoint, const Point currentPoint, const Frame& bounds,
	const double minSize)
{
	const auto [left, right] = CalculateCreatedRange(startPoint.m_x, currentPoint.m_x,
		bounds.m_left, bounds.GetRight(), minSize);
	const auto [top, bottom] = CalculateCreatedRange(startPoint.m_y, currentPoint.m_y,
		bounds.m_top, bounds.GetBottom(), minSize);
	return { left, top, right - left, bottom - top };
}

std::pair<double, double> FrameCalculator::CalculateCreatedRange(double start, double current,
	const double min, const double max, const double minSize)
{
	start = std::clamp(start, min, max);
	current = std::clamp(current, min, max);
	if (current >= start)
	{
		const double high = std::min(std::max(current, start + minSize), max);
		const double low = std::min(start, high - minSize);
		return { low, high };
	}
	const double low = std::max(std::min(current, start - minSize), min);
	const double high = std::max(start, low + minSize);
	return { low, high };
}
