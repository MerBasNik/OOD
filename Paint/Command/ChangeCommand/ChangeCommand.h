//
// Created by Вадим Патрушев on 04.10.2026.
//

#ifndef OOD_CHANGECOMMAND_H
#define OOD_CHANGECOMMAND_H

#include "Command/ICommand.h"

class ChangeCommand : public ICommand
{
public:
	ChangeCommand(std::stringstream& input);
	void Execute(Picture& picture, ICanvas& canvas) override;

private:
	std::string m_id;
	std::string m_type;
	std::string m_params;
};

#endif //OOD_CHANGECOMMAND_H
