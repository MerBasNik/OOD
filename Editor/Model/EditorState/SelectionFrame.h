//
// Created by Вадим Патрушев on 15.10.2026.
//

#ifndef OOD_EDITOR_SELECTIONFRAME_H
#define OOD_EDITOR_SELECTIONFRAME_H

#include "Enums/ResizeHandle.h"
#include "Model/Types/Frame.h"
#include <array>
#include <optional>

class SelectionFrame
{
public:
	static constexpr double HANDLE_SIZE = 8;
	static constexpr double BORDER_SELECT_SIZE = 4;

	static constexpr std::array<ResizeHandle, 8> ALL_HANDLES = {
		ResizeHandle::TopLeft,
		ResizeHandle::Top,
		ResizeHandle::TopRight,
		ResizeHandle::Right,
		ResizeHandle::BottomRight,
		ResizeHandle::Bottom,
		ResizeHandle::BottomLeft,
		ResizeHandle::Left,
	};

	static Frame GetHandleFrame(const Frame& selection, ResizeHandle handle);
	static std::optional<ResizeHandle> HitTestHandle(const Frame& selection, Point point);
	static bool HitTestBorder(const Frame& selection, Point point);

	static bool IsLeftMove(ResizeHandle handle);
	static bool IsRightMove(ResizeHandle handle);
	static bool IsTopMove(ResizeHandle handle);
	static bool IsBottomMove(ResizeHandle handle);

private:
	static Point GetHandleCenter(const Frame& selection, ResizeHandle handle);
};

#endif // OOD_EDITOR_SELECTIONFRAME_H
