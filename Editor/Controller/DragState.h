//
// Created by Вадим Патрушев on 18.10.2026.
//

#ifndef OOD_EDITOR_DRAGSTATE_H
#define OOD_EDITOR_DRAGSTATE_H

#include "Enums/DragMode.h"
#include "Model/EditorState/SelectionGeometry.h"
#include "Model/Types/Frame.h"

struct DragState
{
	DragMode m_mode = DragMode::None;
	int m_shapeId = 0;
	ResizeHandle m_handle = ResizeHandle::TopLeft;
	Point m_startPoint{};
	Frame m_startFrame{};
};

#endif // OOD_EDITOR_DRAGSTATE_H
