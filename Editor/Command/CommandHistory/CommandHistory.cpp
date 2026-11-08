//
// Created by Вадим Патрушев on 26.10.2026.
//

#include "CommandHistory.h"
#include <stdexcept>

CommandHistory::CommandHistory(const size_t maxSize)
	: m_maxSize(maxSize)
{
	if (m_maxSize == 0)
	{
		throw std::invalid_argument("Размер истории должен быть положительным");
	}
}

void CommandHistory::Execute(std::unique_ptr<ICommand> command)
{
	if (!command)
	{
		throw std::invalid_argument("Команда не задана");
	}
	command->Execute();
	m_commands.resize(m_executedCount);
	m_commands.push_back(std::move(command));
	if (m_commands.size() > m_maxSize)
	{
		m_commands.pop_front();
	}
	m_executedCount = m_commands.size();
}

std::vector<int> CommandHistory::Undo()
{
	if (!CanUndo())
	{
		return {};
	}
	ICommand& command = *m_commands[m_executedCount - 1];
	command.Unexecute();
	--m_executedCount;
	return command.GetShapeIds();
}

std::vector<int> CommandHistory::Redo()
{
	if (!CanRedo())
	{
		return {};
	}
	ICommand& command = *m_commands[m_executedCount];
	command.Execute();
	++m_executedCount;
	return command.GetShapeIds();
}

void CommandHistory::Clear()
{
	m_commands.clear();
	m_executedCount = 0;
}

bool CommandHistory::CanUndo() const
{
	return m_executedCount > 0;
}

bool CommandHistory::CanRedo() const
{
	return m_executedCount < m_commands.size();
}
