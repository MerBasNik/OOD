//
// Created by Вадим Патрушев on 18.10.2026.
//

#include "ChangeShapeFrameCommand.h"
#include <stdexcept>

ChangeShapeFrameCommand::ChangeShapeFrameCommand(Document& document, const int shapeId, const Frame& frame)
	: m_document(document)
	, m_shapeId(shapeId)
	, m_frame(frame)
{
}

void ChangeShapeFrameCommand::Execute()
{
	const IShape* shape = m_document.FindShape(m_shapeId);
	if (!shape)
	{
		throw std::invalid_argument("Фигура не найдена");
	}
	const Frame oldFrame = shape->GetFrame();
	m_document.SetShapeFrame(m_shapeId, m_frame);
	m_oldFrame = oldFrame;
}

void ChangeShapeFrameCommand::Unexecute()
{
	if (!m_oldFrame)
	{
		throw std::logic_error("Изменение фигуры не выполнено");
	}
	m_document.SetShapeFrame(m_shapeId, *m_oldFrame);
}
