//
// Created by Вадим Патрушев on 15.10.2026.
//

#ifndef OOD_EDITOR_EDITORCONTROLLER_H
#define OOD_EDITOR_EDITORCONTROLLER_H

#include "DragState.h"
#include "Enums/CursorType.h"
#include "HitResult.h"
#include "Model/Document/Document.h"
#include "Model/EditorState/EditorState.h"
#include <filesystem>
#include <optional>

class EditorController
{
public:
	EditorController(Document& document, EditorState& state);

	void AddShape(ShapeType type);
	void DeleteSelectedShape();
	void SelectShapeTool(std::optional<ShapeType> type);
	void Cancel();

	void OnMouseDown(Point point);
	void OnMouseMove(Point point);
	void OnMouseUp(Point point);

	HitResult HitTest(Point point) const;
	CursorType GetCursorType(Point point) const;

	void NewDocument();
	void SaveDocument(const std::filesystem::path& path);
	void OpenDocument(const std::filesystem::path& path);

private:
	Document& m_document;
	EditorState& m_state;
	DragState m_drag;

	std::optional<Frame> GetSelectedFrame() const;
	void StartDrag(DragMode mode, int id, Point point, ResizeHandle handle = ResizeHandle::TopLeft);
	void StartCreate(Point point);
	void FinishCreate(Point point);
	void FinishDrag(Point point);
	void ResetDrag();
	Frame CalculateDraggedFrame(Point point) const;
	Frame CalculateCreatedFrame(Point point) const;
	static std::unique_ptr<IShape> CreateShape(ShapeType type, const Frame& frame);
	static CursorType GetResizeCursor(ResizeHandle handle);
};

#endif // OOD_EDITOR_EDITORCONTROLLER_H
