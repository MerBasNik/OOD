//
// Created by Вадим Патрушев on 15.10.2026.
//

#ifndef OOD_EDITOR_EDITORCONTROLLER_H
#define OOD_EDITOR_EDITORCONTROLLER_H

#include "Command/CommandHistory/CommandHistory.h"
#include "DragState.h"
#include "Enums/CursorType.h"
#include "HitResult.h"
#include "Model/Document/Document.h"
#include "Model/EditorState/EditorState.h"
#include <optional>

class EditorController : public IDocumentObserver
{
public:
	EditorController(Document& document, EditorState& state, CommandHistory& history);
	~EditorController() override;
	void AddShape(ShapeType type);
	void DeleteSelectedShapes();
	void GroupSelectedShapes();
	void UngroupSelectedShape();
	void SelectShapeTool(std::optional<ShapeType> type);
	void PreviewFillColor(Color color) const;
	void PreviewBorderColor(Color color) const;
	void CommitColors();
	void CancelColors() const;
	void Cancel();
	void Undo();
	void Redo();
	void OnMouseDown(Point point, bool isToggle = false);
	void OnMouseMove(Point point);
	void OnMouseUp(Point point);
	HitResult HitTest(Point point) const;
	CursorType GetCursorType(Point point) const;
	void DocumentUpdate() override;

private:
	Document& m_document;
	EditorState& m_state;
	CommandHistory& m_history;
	DragState m_drag;

	std::optional<Frame> GetShapeFrame(int id) const;
	void StartDrag(DragMode mode, int id, Point point, ResizeHandle handle = ResizeHandle::TopLeft);
	void StartCreate(Point point);
	void FinishCreate(Point point);
	void FinishDrag(Point point);
	void ResetPreview();
	void AddSelectShape(std::unique_ptr<IShape> shape) const;
	void SelectShapes(const std::vector<int>& ids) const;
	void RemoveMissingShapesFromState();
	bool IsDraggedShapeMissing() const;
	std::vector<Frame> CalculateDraggedFrames(Point point) const;
	void ExecuteCommands(std::vector<std::unique_ptr<ICommand>> commands) const;
	Frame CalculateCreatedFrame(Point point) const;
	void UpdateCreateDirection(Point point);
	static std::unique_ptr<IShape> CreateShape(ShapeType type, const Frame& frame);
	static CursorType GetResizeCursor(ResizeHandle handle);
};

#endif // OOD_EDITOR_EDITORCONTROLLER_H
