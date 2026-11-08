//
// Created by Вадим Патрушев on 15.10.2026.
//

#include "EditorState.h"
#include <algorithm>

const std::vector<int>& EditorState::GetSelectedShapeIds() const
{
	return m_selectedShapeIds;
}

void EditorState::SetSelectedShapeIds(std::vector<int> ids)
{
	if (m_selectedShapeIds == ids)
	{
		return;
	}
	m_selectedShapeIds = std::move(ids);
	NotifyObservers();
}

bool EditorState::IsShapeSelected(const int id) const
{
	return std::ranges::find(m_selectedShapeIds, id) != m_selectedShapeIds.end();
}

void EditorState::ToggleShapeSelection(const int id)
{
	if (IsShapeSelected(id))
	{
		std::erase(m_selectedShapeIds, id);
	}
	else
	{
		m_selectedShapeIds.push_back(id);
	}
	NotifyObservers();
}

const std::map<int, Frame>& EditorState::GetPreviewFrames() const
{
	return m_previewFrames;
}

std::optional<Frame> EditorState::GetPreviewFrame(const int id) const
{
	const auto it = m_previewFrames.find(id);
	if (it == m_previewFrames.end())
	{
		return std::nullopt;
	}
	return it->second;
}

void EditorState::SetPreviewFrames(std::map<int, Frame> frames)
{
	if (m_previewFrames == frames)
	{
		return;
	}
	m_previewFrames = std::move(frames);
	NotifyObservers();
}

std::optional<Color> EditorState::GetPreviewFillColor() const
{
	return m_previewFillColor;
}

void EditorState::SetPreviewFillColor(const std::optional<Color> color)
{
	if (m_previewFillColor == color)
	{
		return;
	}
	m_previewFillColor = color;
	NotifyObservers();
}

std::optional<Color> EditorState::GetPreviewBorderColor() const
{
	return m_previewBorderColor;
}

void EditorState::SetPreviewBorderColor(const std::optional<Color> color)
{
	if (m_previewBorderColor == color)
	{
		return;
	}
	m_previewBorderColor = color;
	NotifyObservers();
}

std::optional<ShapeType> EditorState::GetShapeTool() const
{
	return m_shapeTool;
}

void EditorState::SetShapeTool(const std::optional<ShapeType> type)
{
	if (m_shapeTool == type)
	{
		return;
	}
	m_shapeTool = type;
	NotifyObservers();
}

const IShape* EditorState::GetCreatingShape() const
{
	return m_creatingShape.get();
}

void EditorState::SetCreatingShape(std::unique_ptr<IShape> shape)
{
	if (!m_creatingShape && !shape)
	{
		return;
	}
	m_creatingShape = std::move(shape);
	NotifyObservers();
}

// std::optional<std::filesystem::path> EditorState::GetFilePath() const
// {
// 	return m_filePath;
// }
//
// void EditorState::SetFilePath(std::optional<std::filesystem::path> path)
// {
// 	if (m_filePath == path)
// 	{
// 		return;
// 	}
// 	m_filePath = std::move(path);
// 	NotifyObservers();
// }

void EditorState::AddObserver(IEditorStateObserver& observer)
{
	if (std::ranges::find(m_observers, &observer) == m_observers.end())
	{
		m_observers.push_back(&observer);
	}
}

void EditorState::RemoveObserver(IEditorStateObserver& observer)
{
	std::erase(m_observers, &observer);
}

void EditorState::NotifyObservers()
{
	const auto observers = m_observers;
	for (auto* observer : observers)
	{
		observer->EditorStateUpdate();
	}
}
