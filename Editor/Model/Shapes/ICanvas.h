//
// Created by Вадим Патрушев on 15.10.2026.
//

#ifndef OOD_EDITOR_ICANVAS_H
#define OOD_EDITOR_ICANVAS_H

#include "Model/Types/Color.h"
#include "Model/Types/Frame.h"
#include <vector>

class ICanvas
{
public:
	virtual ~ICanvas() = default;
	virtual void DrawRectangle(const Frame& frame, Color color) = 0;
	virtual void DrawEllipse(const Frame& frame, Color color) = 0;
	virtual void DrawPolygon(const std::vector<Point>& vertices, Color color) = 0;
};

#endif // OOD_EDITOR_ICANVAS_H
