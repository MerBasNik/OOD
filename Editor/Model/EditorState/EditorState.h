//
// Created by Вадим Патрушев on 15.10.2026.
//

#ifndef OOD_EDITOR_EDITORSTATE_H
#define OOD_EDITOR_EDITORSTATE_H

#include "IEditorStateObserver.h"
#include "Model/Shapes/IShape.h"
#include "Model/Shapes/ShapeType.h"
#include "Model/Types/Color.h"
#include "Model/Types/Frame.h"
#include <map>
#include <memory>
#include <optional>
#include <vector>

class EditorState
{
public:
	const std::vector<int>& GetSelectedShapeIds() const;
	void SetSelectedShapeIds(std::vector<int> ids);
	bool IsShapeSelected(int id) const;
	void ToggleShapeSelection(int id);
	const std::map<int, Frame>& GetPreviewFrames() const;
	std::optional<Frame> GetPreviewFrame(int id) const;
	void SetPreviewFrames(std::map<int, Frame> frames);
	std::optional<Color> GetPreviewFillColor() const;
	void SetPreviewFillColor(std::optional<Color> color);
	std::optional<Color> GetPreviewBorderColor() const;
	void SetPreviewBorderColor(std::optional<Color> color);
	std::optional<ShapeType> GetShapeTool() const;
	void SetShapeTool(std::optional<ShapeType> type);
	const IShape* GetCreatingShape() const;
	void SetCreatingShape(std::unique_ptr<IShape> shape);
	void AddObserver(IEditorStateObserver& observer);
	void RemoveObserver(IEditorStateObserver& observer);

private:
	std::vector<int> m_selectedShapeIds;
	std::map<int, Frame> m_previewFrames;
	std::optional<Color> m_previewFillColor;
	std::optional<Color> m_previewBorderColor;
	std::optional<ShapeType> m_shapeTool;
	std::unique_ptr<IShape> m_creatingShape;
	std::vector<IEditorStateObserver*> m_observers;

	void NotifyObservers();
};

#endif // OOD_EDITOR_EDITORSTATE_H
