//
// Created by Вадим Патрушев on 26.10.2026.
//

#include "ChangeShapeIndexCommand.h"

ChangeShapeIndexCommand::ChangeShapeIndexCommand(Document& document, const int shapeId, const size_t index)
	: m_document(document)
	, m_shapeId(shapeId)
	, m_index(index)
{
}

void ChangeShapeIndexCommand::Execute()
{
	m_document.SetShapeIndex(m_shapeId, m_index);
}
