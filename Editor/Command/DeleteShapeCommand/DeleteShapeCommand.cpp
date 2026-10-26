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
	const auto index = m_document.FindShapeIndex(m_shapeId);
	if (!index)
	{
		return;
	}
	m_index = *index;
	m_shape = m_document.DeleteShape(m_shapeId);
}

void DeleteShapeCommand::Unexecute()
{
	if (m_shape)
	{
		m_document.InsertShape(m_shapeId, std::move(m_shape), m_index);
	}
}
