//
// Created by Вадим Патрушев on 04.10.2026.
//

#ifndef OOD_DELETESHAPECOMMAND_H
#define OOD_DELETESHAPECOMMAND_H

#include "Command/ICommand.h"

class DeleteShapeCommand : public ICommand
{
public:
	DeleteShapeCommand(std::stringstream& input);
	void Execute(Picture& picture, ICanvas& canvas) override;

private:
	std::string m_id;
};

#endif //OOD_DELETESHAPECOMMAND_H
