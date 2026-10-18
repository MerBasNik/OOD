//
// Created by Вадим Патрушев on 15.10.2026.
//

#include "EditorState.h"
#include <algorithm>

std::optional<int> EditorState::GetSelectedShapeId() const
{
	return m_selectedShapeId;
}

void EditorState::SetSelectedShapeId(const std::optional<int> id)
{
	if (m_selectedShapeId == id)
	{
		return;
	}
	m_selectedShapeId = id;
	NotifyObservers();
}

std::optional<Frame> EditorState::GetPreviewFrame() const
{
	return m_previewFrame;
}

void EditorState::SetPreviewFrame(const std::optional<Frame> frame)
{
	if (m_previewFrame == frame)
	{
		return;
	}
	m_previewFrame = frame;
	NotifyObservers();
}

std::optional<std::filesystem::path> EditorState::GetFilePath() const
{
	return m_filePath;
}

void EditorState::SetFilePath(std::optional<std::filesystem::path> path)
{
	if (m_filePath == path)
	{
		return;
	}
	m_filePath = std::move(path);
	NotifyObservers();
}

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
