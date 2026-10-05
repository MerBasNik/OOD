//
// Created by Вадим Патрушев on 04.10.2026.
//

#ifndef OOD_ICOMMAND_H
#define OOD_ICOMMAND_H

#include "Picture.h"

class ICommand
{
public:
	virtual ~ICommand() = default;
	virtual void Execute(Picture& picture, ICanvas& canvas) = 0;
};

#endif // OOD_ICOMMAND_H
