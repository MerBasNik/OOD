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
	const auto index = m_document.FindShapeIndex(m_groupId);
	if (!index)
	{
		throw std::invalid_argument("Фигура не найдена");
	}
	if (!m_document.FindShape(m_groupId)->GetGroup())
	{
		throw std::invalid_argument("Фигура не является группой");
	}

	m_index = *index;
	m_group = m_document.DeleteShape(m_groupId);
	auto children = m_group->GetGroup()->ReleaseChildren();
	const bool isFirstExecute = m_childIds.empty();
	for (size_t i = 0; i < children.size(); ++i)
	{
		if (isFirstExecute)
		{
			m_childIds.push_back(m_document.AddShapeAt(std::move(children[i]), m_index + i));
		}
		else
		{
			m_document.InsertShape(m_childIds[i], std::move(children[i]), m_index + i);
		}
	}
}

void UngroupShapeCommand::Unexecute()
{
	if (!m_group)
	{
		throw std::logic_error("Разгруппировка не выполнена");
	}
	for (const int childId : m_childIds)
	{
		m_group->GetGroup()->AddChild(m_document.DeleteShape(childId));
	}
	m_document.InsertShape(m_groupId, std::move(m_group), m_index);
}

std::vector<int> UngroupShapeCommand::GetShapeIds() const
{
	std::vector<int> ids = m_childIds;
	ids.push_back(m_groupId);
	return ids;
}

const std::vector<int>& UngroupShapeCommand::GetChildIds() const
{
	return m_childIds;
}
