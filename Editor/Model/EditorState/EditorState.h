//
// Created by Вадим Патрушев on 15.10.2026.
//

#ifndef OOD_EDITOR_EDITORSTATE_H
#define OOD_EDITOR_EDITORSTATE_H

#include "IEditorStateObserver.h"
#include "Model/Types/Frame.h"
#include <filesystem>
#include <optional>
#include <vector>

class EditorState
{
public:
	std::optional<int> GetSelectedShapeId() const;
	void SetSelectedShapeId(std::optional<int> id);

	std::optional<Frame> GetPreviewFrame() const;
	void SetPreviewFrame(std::optional<Frame> frame);

	std::optional<std::filesystem::path> GetFilePath() const;
	void SetFilePath(std::optional<std::filesystem::path> path);

	void AddObserver(IEditorStateObserver& observer);
	void RemoveObserver(IEditorStateObserver& observer);

private:
	std::optional<int> m_selectedShapeId;
	std::optional<Frame> m_previewFrame;
	std::optional<std::filesystem::path> m_filePath;
	std::vector<IEditorStateObserver*> m_observers;

	void NotifyObservers();
};

#endif // OOD_EDITOR_EDITORSTATE_H
