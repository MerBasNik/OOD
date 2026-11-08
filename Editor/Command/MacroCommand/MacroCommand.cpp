//
// Created by Вадим Патрушев on 01.11.2026.
//

#include "MacroCommand.h"
#include <algorithm>
#include <stdexcept>

MacroCommand::MacroCommand(std::vector<std::unique_ptr<ICommand>> commands)
	: m_commands(std::move(commands))
{
	if (m_commands.empty())
	{
		throw std::invalid_argument("Список команд пуст");
	}
}

void MacroCommand::Execute()
{
	for (size_t i = 0; i < m_commands.size(); ++i)
	{
		try
		{
			m_commands[i]->Execute();
		}
		catch (...)
		{
			Rollback(i);
			throw;
		}
	}
}

void MacroCommand::Unexecute()
{
	Rollback(m_commands.size());
}

void MacroCommand::Rollback(size_t commandIndex) const
{
	while (commandIndex > 0)
	{
		m_commands[--commandIndex]->Unexecute();
	}
}

std::vector<int> MacroCommand::GetShapeIds() const
{
	std::vector<int> ids;
	for (const auto& command : m_commands)
	{
		for (const int id : command->GetShapeIds())
		{
			if (std::ranges::find(ids, id) == ids.end())
			{
				ids.push_back(id);
			}
		}
	}
	return ids;
}
