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
	m_document.RemoveShape(m_shapeId);
}
