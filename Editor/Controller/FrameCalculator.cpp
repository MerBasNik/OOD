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
	const double minSize, const std::optional<bool> isForwardX, const std::optional<bool> isForwardY)
{
	const auto [left, right] = CalculateCreatedRange(startPoint.m_x, currentPoint.m_x, bounds.m_left, bounds.GetRight(),
		minSize, isForwardX.value_or(currentPoint.m_x >= startPoint.m_x));
	const auto [top, bottom] = CalculateCreatedRange(startPoint.m_y, currentPoint.m_y, bounds.m_top, bounds.GetBottom(),
		minSize, isForwardY.value_or(currentPoint.m_y >= startPoint.m_y));
	return { left, top, right - left, bottom - top };
}

std::optional<bool> FrameCalculator::IsCreateForward(const double start, const double current, const double minSize,
	const std::optional<bool> wasForward)
{
	if (wasForward)
	{
		return wasForward;
	}
	if (current - start >= minSize)
	{
		return true;
	}
	if (start - current >= minSize)
	{
		return false;
	}
	return std::nullopt;
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
	const ResizeHandle handle, const Point offset, const Frame& bounds, const double minSize, const bool scalePositions)
{
	const Frame& startFrame = startFrames.at(index);
	const Frame resizedFrame = CalculateResizedFrame(startFrame, handle, offset, bounds, minSize);
	const double rawScaleX = resizedFrame.m_width / startFrame.m_width;
	const double rawScaleY = resizedFrame.m_height / startFrame.m_height;
	const bool isLeftMove = SelectionFrame::IsLeftMove(handle);
	const bool isTopMove = SelectionFrame::IsTopMove(handle);
	double scaleX = rawScaleX;
	double scaleY = rawScaleY;
	Point groupAnchor{};
	if (scalePositions)
	{
		const Frame group = GetBoundingFrame(startFrames);
		groupAnchor = { isLeftMove ? group.GetRight() : group.m_left, isTopMove ? group.GetBottom() : group.m_top };
		scaleX = ClampScale(scaleX, group.m_left, group.GetRight(), isLeftMove, bounds.m_left, bounds.GetRight(), 0);
		scaleY = ClampScale(scaleY, group.m_top, group.GetBottom(), isTopMove, bounds.m_top, bounds.GetBottom(), 0);
	}
	for (const auto& frame : startFrames)
	{
		const double minWidth = scalePositions ? std::min(minSize, frame.m_width) : minSize;
		const double minHeight = scalePositions ? std::min(minSize, frame.m_height) : minSize;
		scaleX = ClampScale(scaleX, frame.m_left, frame.GetRight(), isLeftMove, bounds.m_left, bounds.GetRight(), minWidth);
		scaleY = ClampScale(scaleY, frame.m_top, frame.GetBottom(), isTopMove, bounds.m_top, bounds.GetBottom(), minHeight);
	}

	std::vector<Frame> frames;
	frames.reserve(startFrames.size());
	for (const auto& frame : startFrames)
	{
		Point anchor;
		if (scalePositions)
		{
			anchor = groupAnchor;
		}
		else
		{
			anchor = Point{
				isLeftMove ? frame.GetRight() : frame.m_left,
				isTopMove ? frame.GetBottom() : frame.m_top
			};
		}
		frames.push_back(ScaleFrame(frame, scaleX, scaleY, anchor));
	}
	if (!scalePositions && scaleX == rawScaleX && scaleY == rawScaleY)
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
	const double min, const double max, const double minSize, const bool isForward)
{
	start = std::clamp(start, min, max);
	current = std::clamp(current, min, max);
	if (isForward)
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

Frame FrameCalculator::ScaleFrame(const Frame& frame, const double scaleX, const double scaleY, const Point anchor)
{
	return { anchor.m_x + (frame.m_left - anchor.m_x) * scaleX,
		anchor.m_y + (frame.m_top - anchor.m_y) * scaleY,
		frame.m_width * scaleX, frame.m_height * scaleY };
}
