//
// Created by Вадим Патрушев on 12.10.2026.
//

#include "CircleBehavior.h"
#include "LineBehavior.h"
#include "Picture.h"
#include "Shape.h"
#include <gtest/gtest.h>

class ClonePictureTests : public ::testing::Test
{
public:
	void AddCircle(Picture& picture, const std::string& id, const double x, const double y, const double r)
	{
		picture.AddShape(std::make_unique<Shape>(id, 0x000000, std::make_unique<CircleBehavior>(Point{ x, y }, r)));
	}

	void AddLine(Picture& picture, const std::string& id, const double x1, const double y1, const double x2, const double y2)
	{
		picture.AddShape(std::make_unique<Shape>(id, 0x000000, std::make_unique<LineBehavior>(Point{ x1, y1 }, Point{ x2, y2 })));
	}
};

TEST_F(ClonePictureTests, ClonePicture)
{
	Picture picture;
	PictureObserver observer;
	picture.AddObserver(observer);

	AddCircle(picture, "sh1", 0, 0, 10);
	auto newPicture = picture.Clone();
	newPicture->ChangeColor("sh1", 0xffffff);

	EXPECT_EQ(observer.GetCountCalls(), 1);
}

TEST_F(ClonePictureTests, AddObserverToClonePicture)
{
	Picture picture;
	PictureObserver observer;
	PictureObserver newObserver;
	picture.AddObserver(observer);

	AddCircle(picture, "sh1", 0, 0, 10);
	auto newPicture = picture.Clone();
	newPicture->AddObserver(newObserver);

	picture.ChangeColor("sh1", 0xffffff);
	EXPECT_EQ(observer.GetCountCalls(), 2);
	EXPECT_EQ(newObserver.GetCountCalls(), 0);

	newPicture->ChangeColor("sh1", 0xffffff);
	EXPECT_EQ(observer.GetCountCalls(), 2);
	EXPECT_EQ(newObserver.GetCountCalls(), 1);
}

TEST_F(ClonePictureTests, ChangeShapeFromCloneFromPtr)
{
	Picture picture;
	PictureObserver observer;
	PictureObserver newObserver;
	picture.AddObserver(observer);

	AddCircle(picture, "sh1", 0, 0, 10);
	auto newPicture = picture.Clone();
	newPicture->AddObserver(newObserver);

	auto* shape = newPicture->GetShape("sh1");
	shape->ChangeColor(0xffffff);

	EXPECT_EQ(observer.GetCountCalls(), 1);
	EXPECT_EQ(newObserver.GetCountCalls(), 1);
}

TEST_F(ClonePictureTests, ChangeShapeFromOriginalFromPtr)
{
	Picture picture;
	PictureObserver observer;
	PictureObserver newObserver;
	picture.AddObserver(observer);

	AddCircle(picture, "sh1", 0, 0, 10);
	auto newPicture = picture.Clone();
	newPicture->AddObserver(newObserver);

	auto* shape = picture.GetShape("sh1");
	shape->ChangeColor(0xffffff);

	EXPECT_EQ(observer.GetCountCalls(), 2);
	EXPECT_EQ(newObserver.GetCountCalls(), 0);
}

TEST_F(ClonePictureTests, AddShapeToClone)
{
	Picture picture;
	PictureObserver observer;
	PictureObserver newObserver;
	picture.AddObserver(observer);

	AddCircle(picture, "sh1", 0, 0, 10);
	auto newPicture = picture.Clone();
	newPicture->AddObserver(newObserver);
	AddCircle(*newPicture, "sh2", 0, 0, 10);

	EXPECT_EQ(observer.GetCountCalls(), 1);
	EXPECT_EQ(newObserver.GetCountCalls(), 1);
	EXPECT_EQ(picture.GetShapesCount(), 1);
	EXPECT_EQ(newPicture->GetShapesCount(), 2);
}

TEST_F(ClonePictureTests, AddShapeToOriginal)
{
	Picture picture;
	PictureObserver observer;
	PictureObserver newObserver;
	picture.AddObserver(observer);

	AddCircle(picture, "sh1", 0, 0, 10);
	auto newPicture = picture.Clone();
	newPicture->AddObserver(newObserver);
	AddCircle(picture, "sh2", 0, 0, 10);

	EXPECT_EQ(observer.GetCountCalls(), 2);
	EXPECT_EQ(newObserver.GetCountCalls(), 0);
	EXPECT_EQ(picture.GetShapesCount(), 2);
	EXPECT_EQ(newPicture->GetShapesCount(), 1);
}

TEST_F(ClonePictureTests, DeleteShapeFromOriginal)
{
	Picture picture;
	PictureObserver observer;
	PictureObserver newObserver;
	picture.AddObserver(observer);

	AddCircle(picture, "sh1", 0, 0, 10);
	auto newPicture = picture.Clone();
	newPicture->AddObserver(newObserver);
	picture.DeleteShape("sh1");

	EXPECT_EQ(observer.GetCountCalls(), 2);
	EXPECT_EQ(newObserver.GetCountCalls(), 0);
	EXPECT_EQ(picture.GetShapesCount(), 0);
	EXPECT_EQ(newPicture->GetShapesCount(), 1);
}

TEST_F(ClonePictureTests, DeleteShapeFromClone)
{
	Picture picture;
	PictureObserver observer;
	PictureObserver newObserver;
	picture.AddObserver(observer);

	AddCircle(picture, "sh1", 0, 0, 10);
	auto newPicture = picture.Clone();
	newPicture->AddObserver(newObserver);
	newPicture->DeleteShape("sh1");

	EXPECT_EQ(observer.GetCountCalls(), 1);
	EXPECT_EQ(newObserver.GetCountCalls(), 1);
	EXPECT_EQ(picture.GetShapesCount(), 1);
	EXPECT_EQ(newPicture->GetShapesCount(), 0);
}