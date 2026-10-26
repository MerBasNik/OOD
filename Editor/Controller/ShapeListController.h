//
// Created by Вадим Патрушев on 26.10.2026.
//

#ifndef OOD_EDITOR_SHAPELISTCONTROLLER_H
#define OOD_EDITOR_SHAPELISTCONTROLLER_H

#include "Command/CommandHistory/CommandHistory.h"
#include "Model/Document/Document.h"
#include "Model/EditorState/EditorState.h"
#include <optional>

class ShapeListController
{
public:
	ShapeListController(Document& document, EditorState& state, CommandHistory& history);
	void OnMouseDown(std::optional<size_t> row);
	void OnMouseMove(size_t slot);
	void OnMouseUp(size_t slot);
	void Cancel();
	std::optional<size_t> GetDropSlot() const;

private:
	Document& m_document;
	EditorState& m_state;
	CommandHistory& m_history;
	std::optional<int> m_draggedId;
	std::optional<size_t> m_dropSlot;

	std::optional<size_t> GetTargetRow(size_t slot) const;
	size_t RowToIndex(size_t row) const;
};

#endif // OOD_EDITOR_SHAPELISTCONTROLLER_H
