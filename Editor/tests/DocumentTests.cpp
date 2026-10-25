//
// Created by Вадим Патрушев on 15.10.2026.
//

#include "Mock/MockDocumentObserver.h"
#include "Model/Document/Document.h"
#include "Model/Shapes/Ellipse.h"
#include "Model/Shapes/Rectangle.h"
#include <gtest/gtest.h>

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
	MockDocumentObserver observer;
	document.AddObserver(observer);
	EXPECT_CALL(observer, DocumentUpdate).Times(1);

	document.AddShape(MakeRectangle({ 0, 0, 100, 100 }));
}

TEST_F(DocumentTest, NotifiesObserverAboutChangedFrame)
{
	const auto id = document.AddShape(MakeRectangle({ 0, 0, 100, 100 }));
	MockDocumentObserver observer;
	document.AddObserver(observer);
	EXPECT_CALL(observer, DocumentUpdate).Times(1);

	document.SetShapeFrame(id, { 10, 0, 100, 100 });
}

TEST_F(DocumentTest, DoesNotNotifyWhenFrameIsUnchanged)
{
	const auto id = document.AddShape(MakeRectangle({ 0, 0, 100, 100 }));
	MockDocumentObserver observer;
	document.AddObserver(observer);
	EXPECT_CALL(observer, DocumentUpdate).Times(0);

	document.SetShapeFrame(id, { 0, 0, 100, 100 });
}

TEST_F(DocumentTest, NotifiesObserverAboutRemovedShape)
{
	const auto id = document.AddShape(MakeRectangle({ 0, 0, 100, 100 }));
	MockDocumentObserver observer;
	document.AddObserver(observer);
	EXPECT_CALL(observer, DocumentUpdate).Times(1);

	document.RemoveShape(id);
}

TEST_F(DocumentTest, RemovedObserverIsNotNotified)
{
	MockDocumentObserver observer;
	document.AddObserver(observer);
	document.RemoveObserver(observer);
	EXPECT_CALL(observer, DocumentUpdate).Times(0);

	document.AddShape(MakeRectangle({ 0, 0, 100, 100 }));
	document.Clear();
}

TEST_F(DocumentTest, SetShapeIndexMovesShapeUp)
{
	const auto first = document.AddShape(MakeRectangle({ 0, 0, 10, 10 }));
	const auto second = document.AddShape(MakeRectangle({ 0, 0, 10, 10 }));
	const auto third = document.AddShape(MakeRectangle({ 0, 0, 10, 10 }));

	document.SetShapeIndex(first, 2);

	EXPECT_EQ(document.GetShapeId(0), second);
	EXPECT_EQ(document.GetShapeId(1), third);
	EXPECT_EQ(document.GetShapeId(2), first);
}

TEST_F(DocumentTest, SetShapeIndexMovesShapeDown)
{
	const auto first = document.AddShape(MakeRectangle({ 0, 0, 10, 10 }));
	const auto second = document.AddShape(MakeRectangle({ 0, 0, 10, 10 }));
	const auto third = document.AddShape(MakeRectangle({ 0, 0, 10, 10 }));

	document.SetShapeIndex(third, 0);

	EXPECT_EQ(document.GetShapeId(0), third);
	EXPECT_EQ(document.GetShapeId(1), first);
	EXPECT_EQ(document.GetShapeId(2), second);
}

TEST_F(DocumentTest, SetShapeIndexChangesHitTestOrder)
{
	const auto bottom = document.AddShape(MakeRectangle({ 0, 0, 100, 100 }));
	document.AddShape(MakeRectangle({ 50, 50, 100, 100 }));

	document.SetShapeIndex(bottom, 1);

	EXPECT_EQ(document.FindTopShape({ 75, 75 }), bottom);
}

TEST_F(DocumentTest, SetShapeIndexRejectsUnknownShapeAndInvalidIndex)
{
	const auto id = document.AddShape(MakeRectangle({ 0, 0, 10, 10 }));

	EXPECT_THROW(document.SetShapeIndex(id + 1, 0), std::invalid_argument);
	EXPECT_THROW(document.SetShapeIndex(id, 1), std::out_of_range);
}

TEST_F(DocumentTest, NotifiesObserverAboutChangedIndex)
{
	const auto id = document.AddShape(MakeRectangle({ 0, 0, 10, 10 }));
	document.AddShape(MakeRectangle({ 0, 0, 10, 10 }));
	MockDocumentObserver observer;
	document.AddObserver(observer);
	EXPECT_CALL(observer, DocumentUpdate).Times(1);

	document.SetShapeIndex(id, 1);
}

TEST_F(DocumentTest, DoesNotNotifyWhenIndexIsUnchanged)
{
	const auto id = document.AddShape(MakeRectangle({ 0, 0, 10, 10 }));
	MockDocumentObserver observer;
	document.AddObserver(observer);
	EXPECT_CALL(observer, DocumentUpdate).Times(0);

	document.SetShapeIndex(id, 0);
}

TEST_F(DocumentTest, FindShapeIndexReturnsPositionOfShape)
{
	document.AddShape(MakeRectangle({ 0, 0, 10, 10 }));
	const auto id = document.AddShape(MakeRectangle({ 0, 0, 10, 10 }));

	EXPECT_EQ(document.FindShapeIndex(id), 1u);
	EXPECT_EQ(document.FindShapeIndex(id + 1), std::nullopt);
}
