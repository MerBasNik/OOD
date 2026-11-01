//
// Created by Вадим Патрушев on 18.10.2026.
//

#ifndef OOD_EDITOR_DRAGSTATE_H
#define OOD_EDITOR_DRAGSTATE_H

#include "Enums/DragMode.h"
#include "Model/EditorState/Enums/ResizeHandle.h"
#include "Model/Types/Frame.h"
#include <vector>

struct DragState
{
	DragMode m_mode = DragMode::None;
	ResizeHandle m_handle = ResizeHandle::TopLeft;
	Point m_startPoint{};
	std::vector<int> m_shapeIds;
	std::vector<Frame> m_startFrames;
	size_t m_shapeIndex = 0;
};

#endif // OOD_EDITOR_DRAGSTATE_H
