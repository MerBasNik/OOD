//
// Created by Вадим Патрушев on 18.10.2026.
//

#ifndef OOD_EDITOR_FRAMECALCULATOR_H
#define OOD_EDITOR_FRAMECALCULATOR_H

#include "Model/EditorState/Enums/ResizeHandle.h"
#include "Model/Types/Frame.h"

class FrameCalculator
{
public:
	static Frame CalculateMovedFrame(const Frame& startFrame, Point offset, const Frame& bounds);
	static Frame CalculateResizedFrame(const Frame& startFrame, ResizeHandle handle, Point offset,
		const Frame& bounds, double minSize);

private:
	static double Clamp(double value, double lo, double hi);
};

#endif // OOD_EDITOR_FRAMECALCULATOR_H
