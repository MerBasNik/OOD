//
// Created by Вадим Патрушев on 04.10.2026.
//

#ifndef OOD_CLONESHAPECOMMAND_H
#define OOD_CLONESHAPECOMMAND_H
#include "Command/ICommand.h"

class CloneShapeCommand : public ICommand
{
public:
	explicit CloneShapeCommand(std::stringstream& input);
	void Execute(Picture& picture, ICanvas& canvas) override;

private:
	std::string m_id;
	std::string m_newId;
};

#endif //OOD_CLONESHAPECOMMAND_H
