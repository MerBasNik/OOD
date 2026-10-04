//
// Created by Вадим Патрушев on 04.10.2026.
//

#ifndef OOD_CLONECOMMAND_H
#define OOD_CLONECOMMAND_H
#include "Command/ICommand.h"

class CloneCommand : public ICommand
{
public:
	CloneCommand(std::stringstream& input);
	void Execute(Picture& picture, ICanvas& canvas) override;

private:
	std::string m_id;
	std::string m_newId;
};

#endif //OOD_CLONECOMMAND_H
