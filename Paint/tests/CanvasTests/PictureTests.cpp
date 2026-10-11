//
// Created by Вадим Патрушев on 05.10.2026.
//

#include "CircleBehavior.h"
#include "LineBehavior.h"
#include "MockCanvas.h"
#include "Picture.h"
#include "Shape.h"
#include <SFML/Graphics/Shape.hpp>
#include <gtest/gtest.h>

using ::testing::_;

class PictureTest : public ::testing::Test
{
public:
	Picture picture;
	MockCanvas canvas;

	void AddCircle(const std::string& id, const double x, const double y, const double r)
	{
		picture.AddShape(std::make_unique<Shape>(id, 0x000000, std::make_unique<CircleBehavior>(Point{ x, y }, r)));
	}

	void AddLine(const std::string& id, const double x1, const double y1, const double x2, const double y2)
	{
		picture.AddShape(std::make_unique<Shape>(id, 0x000000, std::make_unique<LineBehavior>(Point{ x1, y1 }, Point{ x2, y2 })));
	}
};

TEST_F(PictureTest, AddShape)
{
	AddCircle("sh1", 10, 20, 5);

	EXPECT_NE(picture.GetShape("sh1"), nullptr);
}

TEST_F(PictureTest, GetNotExistsShape)
{
	EXPECT_EQ(picture.GetShape("sh2"), nullptr);
}

TEST_F(PictureTest, DeleteShape)
{
	AddCircle("sh1", 0, 0, 5);
	AddCircle("sh2", 10, 10, 5);

	picture.DeleteShape("sh1");

	EXPECT_EQ(picture.GetShape("sh1"), nullptr);
	EXPECT_NE(picture.GetShape("sh2"), nullptr);
}

TEST_F(PictureTest, DeleteNotExistsShape)
{
	AddCircle("sh1", 0, 00, 5);

	picture.DeleteShape("sh2");

	EXPECT_NE(picture.GetShape("sh1"), nullptr);
}

TEST_F(PictureTest, MoveShape)
{
	AddCircle("sh1", 0, 0, 5);
	AddCircle("sh2", 20, 30, 5);

	picture.MoveShape("sh1", { 10, 15 });

	EXPECT_CALL(canvas, SetColor(0x000000));
	EXPECT_CALL(canvas, DrawEllipse(Point{ 10, 15 }, Point{ 5, 5 }));
	picture.DrawShape("sh1", canvas);

	EXPECT_CALL(canvas, SetColor(0x000000));
	EXPECT_CALL(canvas, DrawEllipse(Point{ 20, 30 }, Point{ 5, 5 }));
	picture.DrawShape("sh2", canvas);
}

TEST_F(PictureTest, MovePicture)
{
	AddCircle("sh1", 0, 0, 5);
	AddCircle("sh2", 20, 30, 5);

	picture.MovePicture({ 10, 15 });

	EXPECT_CALL(canvas, SetColor(0x000000)).Times(2);
	EXPECT_CALL(canvas, DrawEllipse(Point{ 10, 15 }, Point{ 5, 5 }));
	EXPECT_CALL(canvas, DrawEllipse(Point{ 30, 45 }, Point{ 5, 5 }));

	picture.DrawPicture(canvas);
}

TEST_F(PictureTest, DrawNotExistsShape)
{
	EXPECT_CALL(canvas, SetColor(0x000000)).Times(0);
	picture.DrawShape("sh1", canvas);
}

TEST_F(PictureTest, ChangeColorShape)
{
	AddCircle("sh1", 0, 0, 5);
	AddCircle("sh2", 20, 30, 5);

	picture.ChangeColor("sh1", 0xffffff);

	EXPECT_EQ(picture.GetShape("sh1")->GetColor(), 0xffffff);
	EXPECT_EQ(picture.GetShape("sh2")->GetColor(), 0x000000);
}

TEST_F(PictureTest, ChangeShapeBehavior)
{
	AddCircle("sh1", 0, 0, 5);

	auto lineBehavior = std::make_unique<LineBehavior>(Point{ 0, 0 }, Point{ 10, 10 });
	picture.ChangeShape("sh1", std::move(lineBehavior));

	EXPECT_CALL(canvas, SetColor(0x000000));
	EXPECT_CALL(canvas, MoveTo(Point{ 0, 0 }));
	EXPECT_CALL(canvas, LineTo(Point{ 10, 10 }));

	picture.DrawShape("sh1", canvas);
}

TEST_F(PictureTest, ChangeShape)
{
	AddCircle("sh1", 0, 0, 5);

	auto lineBehavior = std::make_unique<LineBehavior>(Point{ 0, 0 }, Point{ 10, 10 });
	picture.ChangeShape("sh1", std::move(lineBehavior));
	const auto* shape = picture.GetShape("sh1");

	EXPECT_EQ(shape->GetId(), "sh1");
	EXPECT_EQ(shape->GetName(), "line");
}

TEST_F(PictureTest, DrawPictures)
{
	AddCircle("sh1", 0, 0, 5);
	AddLine("sh1", 10, 20, 30, 40);

	EXPECT_CALL(canvas, SetColor(0x000000)).Times(2);
	EXPECT_CALL(canvas, DrawEllipse(_, _)).Times(1);
	EXPECT_CALL(canvas, MoveTo(_)).Times(1);
	EXPECT_CALL(canvas, LineTo(_)).Times(1);

	picture.DrawPicture(canvas);
}