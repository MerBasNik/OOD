//
// Created by Вадим Патрушев on 08.11.2026.
//

#include "ChangeShapeTimeIntervalCommand.h"
#include <stdexcept>

ChangeShapeTimeIntervalCommand::ChangeShapeTimeIntervalCommand(Document& document, const int shapeId, const TimeInterval timeInterval)
	: m_document(document)
	, m_shapeId(shapeId)
	, m_timeInterval(timeInterval)
{
}

void ChangeShapeTimeIntervalCommand::Execute()
{
	const auto timeInterval = m_document.GetShapeTimeInterval(m_shapeId);
	m_document.SetShapeTimeInterval(m_shapeId, m_timeInterval);
	m_lastTimeInterval = timeInterval;
}

void ChangeShapeTimeIntervalCommand::Unexecute()
{
	if (!m_lastTimeInterval)
	{
		throw std::logic_error("Изменение фигуры не выполнено");
	}
	m_document.SetShapeTimeInterval(m_shapeId, *m_lastTimeInterval);
}

std::vector<int> ChangeShapeTimeIntervalCommand::GetShapeIds() const
{
	return { m_shapeId };
}
