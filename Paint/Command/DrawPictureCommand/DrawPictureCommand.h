//
// Created by Вадим Патрушев on 04.10.2026.
//

#ifndef OOD_DRAWPICTURECOMMAND_H
#define OOD_DRAWPICTURECOMMAND_H
#include "Command/ICommand.h"

class DrawPictureCommand : public ICommand
{
public:
	DrawPictureCommand() = default;
	void Execute(Picture& picture, ICanvas& canvas) override;
};

#endif //OOD_DRAWPICTURECOMMAND_H
