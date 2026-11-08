//
// Created by Вадим Патрушев on 09.11.2026.
//

#include "GroupShapesCommand.h"
#include "Model/Shapes/GroupShape.h"
#include <algorithm>
#include <stdexcept>

GroupShapesCommand::GroupShapesCommand(Document& document, std::vector<int> shapeIds)
	: m_document(document)
	, m_shapeIds(std::move(shapeIds))
	, m_group(std::make_unique<GroupShape>())
{
	if (m_shapeIds.size() < 2)
	{
		throw std::invalid_argument("Для группы нужно не меньше двух фигур");
	}
}

void GroupShapesCommand::Execute()
{
	if (!m_group)
	{
		throw std::logic_error("Группировка уже выполнена");
	}
	if (!m_groupId)
	{
		SortShapesByLayer();
	}

	std::vector<std::unique_ptr<IShape>> children;
	for (size_t i = m_shapeIds.size(); i-- > 0;)
	{
		children.push_back(m_document.DeleteShape(m_shapeIds[i]));
	}
	for (auto it = children.rbegin(); it != children.rend(); ++it)
	{
		m_group->GetGroup()->AddChild(std::move(*it));
	}

	if (m_groupId)
	{
		m_document.InsertShape(*m_groupId, std::move(m_group), m_groupIndex);
		return;
	}
	m_groupIndex = m_document.GetShapeCount();
	m_groupId = m_document.AddShape(std::move(m_group));
}

void GroupShapesCommand::Unexecute()
{
	if (m_group || !m_groupId)
	{
		throw std::logic_error("Группировка не выполнена");
	}
	m_group = m_document.DeleteShape(*m_groupId);
	auto children = m_group->GetGroup()->ReleaseChildren();
	for (size_t i = 0; i < children.size(); ++i)
	{
		m_document.InsertShape(m_shapeIds[i], std::move(children[i]), m_indices[i]);
	}
}

std::vector<int> GroupShapesCommand::GetShapeIds() const
{
	std::vector<int> ids = m_shapeIds;
	if (m_groupId)
	{
		ids.push_back(*m_groupId);
	}
	return ids;
}

std::optional<int> GroupShapesCommand::GetGroupId() const
{
	return m_groupId;
}

void GroupShapesCommand::SortShapesByLayer()
{
	std::vector<std::pair<size_t, int>> items;
	for (const int id : m_shapeIds)
	{
		const auto index = m_document.FindShapeIndex(id);
		if (!index)
		{
			throw std::invalid_argument("Фигура не найдена");
		}
		items.emplace_back(*index, id);
	}
	std::ranges::sort(items);
	if (std::ranges::adjacent_find(items) != items.end())
	{
		throw std::invalid_argument("Фигура указана несколько раз");
	}
	m_shapeIds.clear();
	m_indices.clear();
	for (const auto& [index, id] : items)
	{
		m_indices.push_back(index);
		m_shapeIds.push_back(id);
	}
}
