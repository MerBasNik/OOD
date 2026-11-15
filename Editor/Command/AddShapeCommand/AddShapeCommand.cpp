//
// Created by Вадим Патрушев on 18.10.2026.
//

#include "AddShapeCommand.h"
#include <stdexcept>

AddShapeCommand::AddShapeCommand(Document& document, std::unique_ptr<IShape> shape, const std::optional<TimeInterval> interval)
	: m_document(document)
	, m_shape(std::move(shape))
{
	if (!m_shape)
	{
		throw std::invalid_argument("Фигура не задана");
	}
	Timeline timeline(document.GetDuration());
	timeline.SetInterval(*m_shape, interval.value_or(TimeInterval{ 0, document.GetDuration() }));
	m_intervals = timeline.GetIntervals(*m_shape);
}

void AddShapeCommand::Execute()
{
	if (!m_shape)
	{
		throw std::logic_error("Добавление фигуры уже выполнено");
	}
	if (m_shapeId)
	{
		m_document.InsertShape(std::move(m_shape), m_index, std::nullopt, m_intervals);
		return;
	}
	m_index = m_document.GetShapeCount();
	const int id = m_shape->GetId();
	m_document.InsertShape(std::move(m_shape), m_index, std::nullopt, m_intervals);
	m_shapeId = id;
}

void AddShapeCommand::Unexecute()
{
	if (m_shape || !m_shapeId)
	{
		throw std::logic_error("Добавление фигуры не выполнено");
	}
	m_intervals = m_document.GetTimeline().GetIntervals(*m_document.FindShape(*m_shapeId));
	m_shape = m_document.DeleteShape(*m_shapeId);
}

std::optional<int> AddShapeCommand::GetShapeId() const
{
	return m_shapeId;
}

std::vector<int> AddShapeCommand::GetShapeIds() const
{
	return m_shapeId ? std::vector{ *m_shapeId } : std::vector<int>{};
}
