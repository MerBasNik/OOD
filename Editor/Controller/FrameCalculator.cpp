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

std::vector<Frame> FrameCalculator::CalculateGroupMovedFrames(const std::vector<Frame>& startFrames, const Point offset, const Frame& bounds)
{
	const Frame group = GetBoundingFrame(startFrames);
	const Frame movedGroup = CalculateMovedFrame(group, offset, bounds);
	std::vector<Frame> frames;
	frames.reserve(startFrames.size());
	for (const auto& frame : startFrames)
	{
		frames.push_back({ movedGroup.m_left + (frame.m_left - group.m_left),
			movedGroup.m_top + (frame.m_top - group.m_top), frame.m_width, frame.m_height });
	}
	return frames;
}

std::vector<Frame> FrameCalculator::CalculateGroupResizedFrames(const std::vector<Frame>& startFrames, const size_t index,
	const ResizeHandle handle, const Point offset, const Frame& bounds, const double minSize)
{
	const Frame& startFrame = startFrames.at(index);
	const Frame resizedFrame = CalculateResizedFrame(startFrame, handle, offset, bounds, minSize);
	const double rawScaleX = resizedFrame.m_width / startFrame.m_width;
	const double rawScaleY = resizedFrame.m_height / startFrame.m_height;
	const bool isLeftMove = SelectionFrame::IsLeftMove(handle);
	const bool isTopMove = SelectionFrame::IsTopMove(handle);
	double scaleX = rawScaleX;
	double scaleY = rawScaleY;
	for (const auto& frame : startFrames)
	{
		scaleX = ClampScale(scaleX, frame.m_left, frame.GetRight(), isLeftMove, bounds.m_left, bounds.GetRight(), minSize);
		scaleY = ClampScale(scaleY, frame.m_top, frame.GetBottom(), isTopMove, bounds.m_top, bounds.GetBottom(), minSize);
	}

	std::vector<Frame> frames;
	frames.reserve(startFrames.size());
	for (const auto& frame : startFrames)
	{
		frames.push_back(ScaleFrame(frame, handle, scaleX, scaleY));
	}
	if (scaleX == rawScaleX && scaleY == rawScaleY)
	{
		frames[index] = resizedFrame;
	}
	return frames;
}

Frame FrameCalculator::GetBoundingFrame(const std::vector<Frame>& frames)
{
	if (frames.empty())
	{
		return {};
	}
	double left = frames.front().m_left;
	double top = frames.front().m_top;
	double right = frames.front().GetRight();
	double bottom = frames.front().GetBottom();
	for (const auto& frame : frames)
	{
		left = std::min(left, frame.m_left);
		top = std::min(top, frame.m_top);
		right = std::max(right, frame.GetRight());
		bottom = std::max(bottom, frame.GetBottom());
	}
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

double FrameCalculator::ClampScale(const double scale, const double low, const double high, const bool isLowMove,
	const double boundsLow, const double boundsHigh, const double minSize)
{
	const double length = high - low;
	const double available = isLowMove ? high - boundsLow : boundsHigh - low;
	return std::clamp(scale, minSize / length, available / length);
}

Frame FrameCalculator::ScaleFrame(const Frame& frame, const ResizeHandle handle, const double scaleX, const double scaleY)
{
	const double width = frame.m_width * scaleX;
	const double height = frame.m_height * scaleY;
	return {
		SelectionFrame::IsLeftMove(handle) ? frame.GetRight() - width : frame.m_left,
		SelectionFrame::IsTopMove(handle) ? frame.GetBottom() - height : frame.m_top,
		width,
		height,
	};
}
