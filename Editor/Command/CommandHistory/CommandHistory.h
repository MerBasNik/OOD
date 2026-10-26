//
// Created by Вадим Патрушев on 26.10.2026.
//

#ifndef OOD_EDITOR_COMMANDHISTORY_H
#define OOD_EDITOR_COMMANDHISTORY_H

#include "Command/ICommand.h"
#include <deque>
#include <memory>

class CommandHistory
{
public:
	static constexpr size_t DEFAULT_MAX_SIZE = 100;

	explicit CommandHistory(size_t maxSize = DEFAULT_MAX_SIZE);
	void Execute(std::unique_ptr<ICommand> command);
	void Undo();
	void Redo();
	void Clear();
	bool CanUndo() const;
	bool CanRedo() const;

private:
	size_t m_maxSize;
	std::deque<std::unique_ptr<ICommand>> m_commands;
	size_t m_executedCount = 0;
};

#endif // OOD_EDITOR_COMMANDHISTORY_H
