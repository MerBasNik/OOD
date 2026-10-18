//
// Created by Вадим Патрушев on 18.10.2026.
//

#ifndef OOD_EDITOR_HITRESULT_H
#define OOD_EDITOR_HITRESULT_H

#include "Enums/HitArea.h"
#include "Model/EditorState/Enums/ResizeHandle.h"
#include <optional>

struct HitResult
{
	HitArea m_area = HitArea::Empty;
	std::optional<int> m_shapeId;
	std::optional<ResizeHandle> m_handle;
};

#endif // OOD_EDITOR_HITRESULT_H
