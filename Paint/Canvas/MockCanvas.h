//
// Created by Вадим Патрушев on 05.10.2026.
//

#ifndef OOD_MOCKCANVAS_H
#define OOD_MOCKCANVAS_H

#include "ICanvas.h"
#include <gmock/gmock.h>

class MockCanvas : public ICanvas
{
public:
	MOCK_METHOD(void, SetColor, (Color color), (override));
	MOCK_METHOD(void, MoveTo, (Point position), (override));
	MOCK_METHOD(void, LineTo, (Point position), (override));
	MOCK_METHOD(void, DrawEllipse, (Point position, Point radiuses), (override));
	MOCK_METHOD(void, DrawText, (Point position, double fontSize, const std::string& text), (override));
};

#endif // OOD_MOCKCANVAS_H
