//
// Created by Вадим Патрушев on 26.10.2026.
//

#include "Mock/MockCanvas.h"
#include "Model/Rendering/RenderShape.h"
#include "Model/Shapes/Ellipse.h"
#include "Model/Shapes/Rectangle.h"
#include "Model/Shapes/Triangle.h"
#include <gtest/gtest.h>

using ::testing::StrictMock;

class ShapeRendererTest : public ::testing::Test
{
public:
	const Frame FRAME{ 10, 20, 100, 50 };
	const Color COLOR = 0x123456;
	StrictMock<MockCanvas> m_canvas;
	RenderShape m_renderer{ m_canvas };
};

TEST_F(ShapeRendererTest, RectangleIsDrawnAsRectangle)
{
	EXPECT_CALL(m_canvas, DrawRectangle(FRAME, COLOR));

	Rectangle(FRAME, COLOR).Accept(m_renderer);
}

TEST_F(ShapeRendererTest, EllipseIsDrawnAsEllipse)
{
	EXPECT_CALL(m_canvas, DrawEllipse(FRAME, COLOR));

	Ellipse(FRAME, COLOR).Accept(m_renderer);
}

TEST_F(ShapeRendererTest, TriangleIsDrawnAsPolygonByVertices)
{
	const std::vector<Point> vertices{ { 60, 20 }, { 110, 70 }, { 10, 70 } };
	EXPECT_CALL(m_canvas, DrawPolygon(vertices, COLOR));

	Triangle(FRAME, COLOR).Accept(m_renderer);
}
