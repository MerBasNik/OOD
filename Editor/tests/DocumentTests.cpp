//
// Created by Вадим Патрушев on 15.10.2026.
//

#include "Mock/MockDocumentObserver.h"
#include "Model/Document/Document.h"
#include "Model/Shapes/Ellipse.h"
#include "Model/Shapes/Rectangle.h"
#include <gtest/gtest.h>

using ::testing::StrictMock;

class DocumentTest : public ::testing::Test
{
public:
	Document document{ 500, 500 };

	std::unique_ptr<IShape> MakeRectangle(const Frame& frame)
	{
		return std::make_unique<Rectangle>(frame, 0);
	}
};

TEST_F(DocumentTest, NewShapeIsPlacedOnTopLayer)
{
	const auto bottom = document.AddShape(MakeRectangle({ 0, 0, 100, 100 }));
	const auto top = document.AddShape(MakeRectangle({ 50, 50, 100, 100 }));

	ASSERT_EQ(document.GetShapeCount(), 2u);
	EXPECT_EQ(document.GetShapeId(0), bottom);
	EXPECT_EQ(document.GetShapeId(1), top);
}

TEST_F(DocumentTest, FindTopShapeReturnsUpperShapeAtPoint)
{
	document.AddShape(MakeRectangle({ 0, 0, 100, 100 }));
	const auto top = document.AddShape(MakeRectangle({ 50, 50, 100, 100 }));

	EXPECT_EQ(document.FindTopShape({ 75, 75 }), top);
}

TEST_F(DocumentTest, FindTopShapeReturnsLowerShapeOutsideUpperOne)
{
	const auto bottom = document.AddShape(MakeRectangle({ 0, 0, 100, 100 }));
	document.AddShape(MakeRectangle({ 50, 50, 100, 100 }));

	EXPECT_EQ(document.FindTopShape({ 25, 25 }), bottom);
}

TEST_F(DocumentTest, FindTopShapeReturnsNothingOnEmptyArea)
{
	document.AddShape(MakeRectangle({ 0, 0, 100, 100 }));

	EXPECT_EQ(document.FindTopShape({ 400, 400 }), std::nullopt);
}

TEST_F(DocumentTest, FindTopShapeUsesRealShapeGeometry)
{
	const auto rectangle = document.AddShape(MakeRectangle({ 0, 0, 100, 100 }));
	document.AddShape(std::make_unique<Ellipse>(Frame{ 0, 0, 100, 100 }, 0));

	EXPECT_EQ(document.FindTopShape({ 3, 3 }), rectangle);
}

TEST_F(DocumentTest, RemoveShapeInvalidatesItsId)
{
	const auto id = document.AddShape(MakeRectangle({ 0, 0, 100, 100 }));

	document.RemoveShape(id);

	EXPECT_EQ(document.GetShapeCount(), 0u);
	EXPECT_EQ(document.FindShape(id), nullptr);
}

TEST_F(DocumentTest, RemoveUnknownShapeDoesNothing)
{
	document.AddShape(MakeRectangle({ 0, 0, 100, 100 }));

	EXPECT_NO_THROW(document.RemoveShape(12345));
	EXPECT_EQ(document.GetShapeCount(), 1u);
}

TEST_F(DocumentTest, AddShapeRejectsShapeOutsideBounds)
{
	EXPECT_THROW(document.AddShape(MakeRectangle({ 450, 0, 100, 100 })), std::invalid_argument);
	EXPECT_EQ(document.GetShapeCount(), 0u);
}

TEST_F(DocumentTest, SetShapeFrameRejectsFrameOutsideBounds)
{
	const auto id = document.AddShape(MakeRectangle({ 0, 0, 100, 100 }));

	EXPECT_THROW(document.SetShapeFrame(id, { -1, 0, 100, 100 }), std::invalid_argument);
	EXPECT_EQ(document.FindShape(id)->GetFrame(), (Frame{ 0, 0, 100, 100 }));
}

TEST_F(DocumentTest, ReplaceShapesIsAtomic)
{
	document.AddShape(MakeRectangle({ 0, 0, 100, 100 }));
	std::vector<std::unique_ptr<IShape>> shapes;
	shapes.push_back(MakeRectangle({ 10, 10, 50, 50 }));
	shapes.push_back(MakeRectangle({ 480, 10, 50, 50 }));

	EXPECT_THROW(document.ReplaceShapes(std::move(shapes)), std::invalid_argument);
	ASSERT_EQ(document.GetShapeCount(), 1u);
	EXPECT_EQ(document.GetShape(0).GetFrame(), (Frame{ 0, 0, 100, 100 }));
}

TEST_F(DocumentTest, NotifiesObserverAboutAddedShape)
{
	StrictMock<MockDocumentObserver> observer;
	document.AddObserver(observer);
	EXPECT_CALL(observer, DocumentUpdate).Times(1);

	document.AddShape(MakeRectangle({ 0, 0, 100, 100 }));
}

TEST_F(DocumentTest, NotifiesObserverAboutChangedFrame)
{
	const auto id = document.AddShape(MakeRectangle({ 0, 0, 100, 100 }));
	StrictMock<MockDocumentObserver> observer;
	document.AddObserver(observer);
	EXPECT_CALL(observer, DocumentUpdate).Times(1);

	document.SetShapeFrame(id, { 10, 0, 100, 100 });
}

TEST_F(DocumentTest, DoesNotNotifyWhenFrameIsUnchanged)
{
	const auto id = document.AddShape(MakeRectangle({ 0, 0, 100, 100 }));
	StrictMock<MockDocumentObserver> observer;
	document.AddObserver(observer);
	EXPECT_CALL(observer, DocumentUpdate).Times(0);

	document.SetShapeFrame(id, { 0, 0, 100, 100 });
}

TEST_F(DocumentTest, NotifiesObserverAboutRemovedShape)
{
	const auto id = document.AddShape(MakeRectangle({ 0, 0, 100, 100 }));
	StrictMock<MockDocumentObserver> observer;
	document.AddObserver(observer);
	EXPECT_CALL(observer, DocumentUpdate).Times(1);

	document.RemoveShape(id);
}

TEST_F(DocumentTest, RemovedObserverIsNotNotified)
{
	StrictMock<MockDocumentObserver> observer;
	document.AddObserver(observer);
	document.RemoveObserver(observer);
	EXPECT_CALL(observer, DocumentUpdate).Times(0);

	document.AddShape(MakeRectangle({ 0, 0, 100, 100 }));
	document.Clear();
}
