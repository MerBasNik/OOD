//
// Created by Вадим Патрушев on 18.10.2026.
//

#ifndef OOD_EDITOR_ICOMMAND_H
#define OOD_EDITOR_ICOMMAND_H

class ICommand
{
public:
	virtual ~ICommand() = default;
	virtual void Execute() = 0;
	virtual void Unexecute() = 0;
};

#endif // OOD_EDITOR_ICOMMAND_H
