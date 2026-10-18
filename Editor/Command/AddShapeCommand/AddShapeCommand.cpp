//
// Created by Вадим Патрушев on 18.10.2026.
//

#include "AddShapeCommand.h"
#include <stdexcept>

AddShapeCommand::AddShapeCommand(Document& document, std::unique_ptr<IShape> shape)
	: m_document(document)
	, m_shape(std::move(shape))
{
	if (!m_shape)
	{
		throw std::invalid_argument("Фигура не задана");
	}
}

void AddShapeCommand::Execute()
{
	if (!m_shape)
	{
		throw std::logic_error("Добавление фигуры уже выполнено");
	}
	m_shapeId = m_document.AddShape(std::move(m_shape));
}

std::optional<int> AddShapeCommand::GetShapeId() const
{
	return m_shapeId;
}
