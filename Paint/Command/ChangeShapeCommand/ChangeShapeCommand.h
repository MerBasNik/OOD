//
// Created by Вадим Патрушев on 04.10.2026.
//

#ifndef OOD_CHANGESHAPECOMMAND_H
#define OOD_CHANGESHAPECOMMAND_H

#include "Command/ICommand.h"

class ChangeShapeCommand : public ICommand
{
public:
	explicit ChangeShapeCommand(std::stringstream& input);
	void Execute(Picture& picture, ICanvas& canvas) override;

private:
	std::string m_id;
	std::string m_type;
	std::string m_params;
};

#endif // OOD_CHANGESHAPECOMMAND_H
