//
// Created by Вадим Патрушев on 26.10.2026.
//

#include "ShapeListController.h"
#include "Command/ChangeShapeIndexCommand/ChangeShapeIndexCommand.h"
#include <algorithm>

ShapeListController::ShapeListController(Document& document, EditorState& state)
	: m_document(document)
	, m_state(state)
{
}

void ShapeListController::OnMouseDown(const std::optional<size_t> row)
{
	Cancel();
	if (!row || *row >= m_document.GetShapeCount())
	{
		m_state.SetSelectedShapeId(std::nullopt);
		return;
	}
	const int id = m_document.GetShapeId(RowToIndex(*row));
	m_state.SetSelectedShapeId(id);
	m_draggedId = id;
}

void ShapeListController::OnMouseMove(const size_t slot)
{
	if (m_draggedId)
	{
		m_dropSlot = slot;
	}
}

void ShapeListController::OnMouseUp(const size_t slot)
{
	const auto draggedId = m_draggedId;
	const auto targetRow = GetTargetRow(slot);
	Cancel();
	if (targetRow)
	{
		ChangeShapeIndexCommand command(m_document, *draggedId, RowToIndex(*targetRow));
		command.Execute();
	}
}

void ShapeListController::Cancel()
{
	m_draggedId = std::nullopt;
	m_dropSlot = std::nullopt;
}

std::optional<size_t> ShapeListController::GetDropSlot() const
{
	if (!m_dropSlot || !GetTargetRow(*m_dropSlot))
	{
		return std::nullopt;
	}
	return m_dropSlot;
}

std::optional<size_t> ShapeListController::GetTargetRow(const size_t slot) const
{
	if (!m_draggedId)
	{
		return std::nullopt;
	}
	const auto index = m_document.FindShapeIndex(*m_draggedId);
	if (!index)
	{
		return std::nullopt;
	}
	const size_t row = m_document.GetShapeCount() - 1 - *index;
	const size_t targetRow = std::min(slot > row ? slot - 1 : slot, m_document.GetShapeCount() - 1);
	if (targetRow == row)
	{
		return std::nullopt;
	}
	return targetRow;
}

size_t ShapeListController::RowToIndex(const size_t row) const
{
	return m_document.GetShapeCount() - 1 - row;
}
