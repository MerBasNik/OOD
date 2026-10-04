//
// Created by Вадим Патрушев on 04.10.2026.
//

#ifndef OOD_ADDCOMMAND_H
#define OOD_ADDCOMMAND_H

#include "../ICommand.h"
#include "Shape.h"

class AddCommand : public ICommand
{
public:
	explicit AddCommand(std::stringstream& input);
	void Execute(Picture& picture, ICanvas& canvas) override;

private:
	std::string m_id;
	Color m_color;
	std::string m_type;
	std::string m_params;
};

#endif //OOD_ADDCOMMAND_H
