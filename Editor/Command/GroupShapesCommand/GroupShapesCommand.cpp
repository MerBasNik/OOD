//
// Created by Вадим Патрушев on 09.11.2026.
//

#include "GroupShapesCommand.h"
#include "Model/Shapes/GroupShape.h"
#include <algorithm>
#include <ranges>
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
		m_groupIndex = m_document.GetShapeCount() - m_shapeIds.size();
	}
	m_groupId = m_document.GroupShapes(m_shapeIds, std::move(m_group), m_groupIndex);
}

void GroupShapesCommand::Unexecute()
{
	if (m_group || !m_groupId)
	{
		throw std::runtime_error("Группировка не выполнена");
	}
	m_group = m_document.UngroupShape(*m_groupId, m_indices);
}

std::vector<int> GroupShapesCommand::GetShapeIds() const
{
	return m_group ? m_shapeIds : std::vector{ *m_groupId };
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

	m_shapeIds.clear();
	m_indices.clear();
	for (const auto& [index, id] : items)
	{
		m_indices.push_back(index);
		m_shapeIds.push_back(id);
	}
}
