//
// Created by Вадим Патрушев on 09.11.2026.
//

#include "UngroupShapeCommand.h"
#include "Model/Shapes/GroupShape.h"
#include <stdexcept>

UngroupShapeCommand::UngroupShapeCommand(Document& document, const int groupId)
	: m_document(document)
	, m_groupId(groupId)
{
}

void UngroupShapeCommand::Execute()
{
	if (m_group)
	{
		throw std::logic_error("Разгруппировка уже выполнена");
	}
	m_parentId = m_document.FindParentShapeId(m_groupId);
	const auto index = m_document.FindShapeIndex(m_groupId, m_parentId);
	if (!index)
	{
		throw std::invalid_argument("Фигура не найдена");
	}
	if (!m_document.FindShape(m_groupId)->GetGroup())
	{
		throw std::invalid_argument("Фигура не является группой");
	}

	m_index = *index;
	const auto* group = m_document.FindShape(m_groupId)->GetGroup();
	m_childIds.clear();
	std::vector<size_t> childIndices;
	for (size_t i = 0; i < group->GetChildCount(); ++i)
	{
		m_childIds.push_back(group->GetChild(i).GetId());
		childIndices.push_back(m_index + i);
	}
	m_group = m_document.UngroupShape(m_groupId, childIndices, m_parentId);
}

void UngroupShapeCommand::Unexecute()
{
	if (!m_group)
	{
		throw std::logic_error("Разгруппировка не выполнена");
	}
	m_document.GroupShapes(m_childIds, std::move(m_group), m_index, m_parentId);
}

std::vector<int> UngroupShapeCommand::GetShapeIds() const
{
	return m_group ? m_childIds : std::vector{ m_groupId };
}

const std::vector<int>& UngroupShapeCommand::GetChildIds() const
{
	return m_childIds;
}
