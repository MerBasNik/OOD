//
// Created by Вадим Патрушев on 04.10.2026.
//

#include "DrawPictureCommand.h"

void DrawPictureCommand::Execute(Picture& picture, ICanvas& canvas)
{
	picture.DrawPicture(canvas);
}