//
// Created by Вадим Патрушев on 28.09.2026.
//

#ifndef OOD_CANVAS_H
#define OOD_CANVAS_H
#include "ICanvas.h"

class Canvas : public ICanvas
{
public:
	Canvas();
	~Canvas() override;
	void SetColor(Color color) override;
	void MoveTop(Point position) override;
	void DrawEllipse(Point position, Point newPosition) override;
	void DrawText(Point position, double fontSize, std::string& text) override;
};

#endif //OOD_CANVAS_H
