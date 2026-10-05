//
// Created by Вадим Патрушев on 04.10.2026.
//

#ifndef OOD_DRAWSHAPECOMMAND_H
#define OOD_DRAWSHAPECOMMAND_H
#include "Command/ICommand.h"

class DrawShapeCommand : public ICommand
{
public:
	explicit DrawShapeCommand(std::stringstream& input);
	void Execute(Picture& picture, ICanvas& canvas) override;

private:
	std::string m_id;
};

#endif // OOD_DRAWSHAPECOMMAND_H
