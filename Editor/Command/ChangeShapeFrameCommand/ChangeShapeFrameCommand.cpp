//
// Created by Вадим Патрушев on 18.10.2026.
//

#include "ChangeShapeFrameCommand.h"

ChangeShapeFrameCommand::ChangeShapeFrameCommand(Document& document, const int shapeId, const Frame& frame)
	: m_document(document)
	, m_shapeId(shapeId)
	, m_frame(frame)
{
}

void ChangeShapeFrameCommand::Execute()
{
	m_document.SetShapeFrame(m_shapeId, m_frame);
}
