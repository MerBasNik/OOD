//
// Created by Вадим Патрушев on 28.09.2026.
//

#ifndef OOD_ICANVAS_H
#define OOD_ICANVAS_H
#include "../Lib/Color/Color.h"
#include "Point.h"

class ICanvas
{
public:
	virtual ~ICanvas() = default;
	virtual void SetColor(Color color);
	virtual void MoveTop(Point position);
	virtual void DrawEllipse(Point position, Point newPosition);
	virtual void DrawText(Point position, double fontSize, std::string& text);
};

#endif //OOD_ICANVAS_H
