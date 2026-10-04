//
// Created by Вадим Патрушев on 04.10.2026.
//

#ifndef OOD_DRAWCOMMAND_H
#define OOD_DRAWCOMMAND_H
#include "Command/ICommand.h"

class DrawCommand : public ICommand
{
public:
	DrawCommand(std::stringstream& input);
	void Execute(Picture& picture, ICanvas& canvas) override;

private:
	std::string m_id;
};

#endif //OOD_DRAWCOMMAND_H
