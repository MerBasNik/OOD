//
// Created by Вадим Патрушев on 26.10.2026.
//

#ifndef OOD_EDITOR_MOCKCANVAS_H
#define OOD_EDITOR_MOCKCANVAS_H

#include "../../View/Canvas/ICanvas.h"
#include <gmock/gmock.h>

class MockCanvas : public ICanvas
{
public:
	MOCK_METHOD(void, DrawRectangle, (const Frame& frame, Color color), (override));
	MOCK_METHOD(void, DrawEllipse, (const Frame& frame, Color color), (override));
	MOCK_METHOD(void, DrawPolygon, (const std::vector<Point>& vertices, Color color), (override));
};

#endif // OOD_EDITOR_MOCKCANVAS_H
