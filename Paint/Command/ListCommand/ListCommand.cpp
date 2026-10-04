//
// Created by Вадим Патрушев on 04.10.2026.
//

#include "ListCommand.h"

void ListCommand::Execute(Picture& picture, ICanvas& canvas)
{
	picture.List();
}