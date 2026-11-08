//
// Created by Вадим Патрушев on 08.11.2026.
//

#ifndef OOD_CHANGESHAPETIMEINTERVALCOMMAND_H
#define OOD_CHANGESHAPETIMEINTERVALCOMMAND_H

#include "Command/ICommand.h"
#include "Model/Document/Document.h"
#include "Model/Types/TimeInterval.h"
#include <optional>

class ChangeShapeTimeIntervalCommand : public ICommand
{
public:
	ChangeShapeTimeIntervalCommand(Document& document, int shapeId, TimeInterval timeInterval);
	void Execute() override;
	void Unexecute() override;
	std::vector<int> GetShapeIds() const override;

private:
	Document& m_document;
	int m_shapeId;
	TimeInterval m_timeInterval;
	std::optional<TimeInterval> m_lastTimeInterval;
};

#endif //OOD_CHANGESHAPETIMEINTERVALCOMMAND_H
