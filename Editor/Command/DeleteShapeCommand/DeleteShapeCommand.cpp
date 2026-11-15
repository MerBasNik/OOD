//
// Created by Вадим Патрушев on 18.10.2026.
//

#include "DeleteShapeCommand.h"

DeleteShapeCommand::DeleteShapeCommand(Document& document, const int shapeId)
	: m_document(document)
	, m_shapeId(shapeId)
{
}

void DeleteShapeCommand::Execute()
{
	if (m_shape)
	{
		return;
	}
	m_parentId = m_document.FindParentShapeId(m_shapeId);
	const auto index = m_document.FindShapeIndex(m_shapeId, m_parentId);
	if (!index)
	{
		return;
	}
	m_index = *index;
	m_shape = m_document.DeleteShape(m_shapeId, m_parentId);
}

void DeleteShapeCommand::Unexecute()
{
	if (m_shape)
	{
		m_document.InsertShape(std::move(m_shape), m_index, m_parentId);
	}
}

std::vector<int> DeleteShapeCommand::GetShapeIds() const
{
	return { m_shapeId };
}
