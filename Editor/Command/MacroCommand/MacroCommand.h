//
// Created by Вадим Патрушев on 01.11.2026.
//

#ifndef OOD_EDITOR_MACROCOMMAND_H
#define OOD_EDITOR_MACROCOMMAND_H

#include "Command/ICommand.h"
#include <memory>
#include <vector>

class MacroCommand : public ICommand
{
public:
	explicit MacroCommand(std::vector<std::unique_ptr<ICommand>> commands);
	void Execute() override;
	void Unexecute() override;

private:
	std::vector<std::unique_ptr<ICommand>> m_commands;

	void Rollback(size_t commandIndex) const;
};

#endif // OOD_EDITOR_MACROCOMMAND_H
