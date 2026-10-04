//
// Created by Вадим Патрушев on 04.10.2026.
//

#ifndef OOD_CHANGECOLORCOMMAND_H
#define OOD_CHANGECOLORCOMMAND_H
#include "Command/ICommand.h"

class ChangeColorCommand : public ICommand
{
public:
	explicit ChangeColorCommand(std::stringstream& input);
	void Execute(Picture& picture, ICanvas& canvas) override;

private:
	std::string m_id;
	Color m_color{};
};

#endif //OOD_CHANGECOLORCOMMAND_H
