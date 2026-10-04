//
// Created by Вадим Патрушев on 04.10.2026.
//

#ifndef OOD_DELETECOMMAND_H
#define OOD_DELETECOMMAND_H

#include "Command/ICommand.h"

class DeleteCommand : public ICommand
{
public:
	DeleteCommand(std::stringstream& input);
	void Execute(Picture& picture, ICanvas& canvas) override;

private:
	std::string m_id;
};

#endif //OOD_DELETECOMMAND_H
