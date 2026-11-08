//
// Created by Вадим Патрушев on 09.11.2026.
//

#ifndef OOD_EDITOR_TIMELINEHITRESULT_H
#define OOD_EDITOR_TIMELINEHITRESULT_H

#include "Controller/Enums/TimelineDragMode.h"
#include <cstddef>
#include <optional>

struct TimelineHitResult
{
	TimelineDragMode m_mode = TimelineDragMode::None;
	std::optional<size_t> m_row;
};

#endif // OOD_EDITOR_TIMELINEHITRESULT_H
