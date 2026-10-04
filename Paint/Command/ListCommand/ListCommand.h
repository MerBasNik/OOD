//
// Created by Вадим Патрушев on 04.10.2026.
//

#ifndef OOD_LISTCOMMAND_H
#define OOD_LISTCOMMAND_H

#include "Command/ICommand.h"

class ListCommand : public ICommand
{
public:
	ListCommand() = default;
	void Execute(Picture& picture, ICanvas& canvas) override;
};

#endif //OOD_LISTCOMMAND_H
