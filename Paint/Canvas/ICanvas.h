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
	virtual void SetColor(Color color) = 0;
	virtual void MoveTo(Point position) = 0;
	virtual void LineTo(Point position) = 0;
	virtual void DrawEllipse(Point position, Point newPosition) = 0;
	virtual void DrawText(Point position, double fontSize, std::string& text) = 0;
};

#endif //OOD_ICANVAS_H
