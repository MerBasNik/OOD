//
// Created by Вадим Патрушев on 18.10.2026.
//

#ifndef OOD_EDITOR_FRAMECALCULATOR_H
#define OOD_EDITOR_FRAMECALCULATOR_H

#include "Model/EditorState/Enums/ResizeHandle.h"
#include "Model/Types/Frame.h"
#include <utility>
#include <vector>

class FrameCalculator
{
public:
	static Frame CalculateMovedFrame(const Frame& startFrame, Point offset, const Frame& bounds);
	static Frame CalculateResizedFrame(const Frame& startFrame, ResizeHandle handle, Point offset, const Frame& bounds, double minSize);
	static Frame CalculateCreatedFrame(Point startPoint, Point currentPoint, const Frame& bounds, double minSize);
	static std::vector<Frame> CalculateGroupMovedFrames(const std::vector<Frame>& startFrames, Point offset, const Frame& bounds);
	static std::vector<Frame> CalculateGroupResizedFrames(const std::vector<Frame>& startFrames, size_t index,
		ResizeHandle handle, Point offset, const Frame& bounds, double minSize);

private:
	static std::pair<double, double> CalculateCreatedRange(double start, double current, double min, double max, double minSize);
	static Frame GetBoundingFrame(const std::vector<Frame>& frames);
	static double ClampScale(double scale, double low, double high, bool isLowMove, double boundsLow, double boundsHigh, double minSize);
	static Frame ScaleFrame(const Frame& frame, ResizeHandle handle, double scaleX, double scaleY);
};

#endif // OOD_EDITOR_FRAMECALCULATOR_H
