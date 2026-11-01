//
// Created by Вадим Патрушев on 01.11.2026.
//

#include "ChangeShapeColorsCommand.h"
#include <stdexcept>

ChangeShapeColorsCommand::ChangeShapeColorsCommand(Document& document, const int shapeId, const Color fillColor, const Color borderColor)
	: m_document(document)
	, m_shapeId(shapeId)
	, m_fillColor(fillColor)
	, m_borderColor(borderColor)
{
}

void ChangeShapeColorsCommand::Execute()
{
	const IShape* shape = m_document.FindShape(m_shapeId);
	if (!shape)
	{
		throw std::invalid_argument("Фигура не найдена");
	}
	const Color oldFillColor = shape->GetFillColor();
	const Color oldBorderColor = shape->GetBorderColor();
	m_document.SetShapeColors(m_shapeId, m_fillColor, m_borderColor);
	m_oldFillColor = oldFillColor;
	m_oldBorderColor = oldBorderColor;
}

void ChangeShapeColorsCommand::Unexecute()
{
	if (!m_oldFillColor || !m_oldBorderColor)
	{
		throw std::logic_error("Изменение цвета фигуры не выполнено");
	}
	m_document.SetShapeColors(m_shapeId, *m_oldFillColor, *m_oldBorderColor);
}
