//
// Created by Вадим Патрушев on 26.10.2026.
//

#include "ChangeShapeIndexCommand.h"
#include <stdexcept>

ChangeShapeIndexCommand::ChangeShapeIndexCommand(Document& document, const int shapeId, const size_t index)
	: m_document(document)
	, m_shapeId(shapeId)
	, m_index(index)
{
}

void ChangeShapeIndexCommand::Execute()
{
	const auto oldIndex = m_document.FindShapeIndex(m_shapeId);
	if (!oldIndex)
	{
		throw std::invalid_argument("Фигура не найдена");
	}
	m_document.SetShapeIndex(m_shapeId, m_index);
	m_oldIndex = oldIndex;
}

void ChangeShapeIndexCommand::Unexecute()
{
	if (!m_oldIndex)
	{
		throw std::logic_error("Изменение порядка фигуры не выполнено");
	}
	m_document.SetShapeIndex(m_shapeId, *m_oldIndex);
}
