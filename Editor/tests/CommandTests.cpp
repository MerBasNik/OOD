//
// Created by Вадим Патрушев on 18.10.2026.
//

#include "Command/AddShapeCommand/AddShapeCommand.h"
#include "Command/ChangeShapeFrameCommand/ChangeShapeFrameCommand.h"
#include "Command/DeleteShapeCommand/DeleteShapeCommand.h"
#include "Model/Shapes/Rectangle.h"
#include <gtest/gtest.h>

class CommandTest : public ::testing::Test
{
public:
	Document document{ 500, 500 };
	static std::unique_ptr<IShape> MakeRectangle(const Frame& frame)
	{
		return std::make_unique<Rectangle>(frame, 0);
	}
};

TEST_F(CommandTest, AddShapeCommandHasNoShapeIdBeforeExecute)
{
	const AddShapeCommand command(document, MakeRectangle({ 10, 10, 50, 50 }));

	EXPECT_EQ(command.GetShapeId(), std::nullopt);
	EXPECT_EQ(document.GetShapeCount(), 0u);
}

TEST_F(CommandTest, AddShapeCommandPlacesShapeOnTopLayer)
{
	document.AddShape(MakeRectangle({ 0, 0, 100, 100 }));
	AddShapeCommand command(document, MakeRectangle({ 10, 10, 50, 50 }));

	command.Execute();

	ASSERT_EQ(document.GetShapeCount(), 2u);
	EXPECT_EQ(document.GetShape(1).GetFrame(), (Frame{ 10, 10, 50, 50 }));
}

TEST_F(CommandTest, AddShapeCommandReturnsIdOfAddedShape)
{
	AddShapeCommand command(document, MakeRectangle({ 10, 10, 50, 50 }));

	command.Execute();

	EXPECT_EQ(command.GetShapeId(), document.GetShapeId(0));
}

TEST_F(CommandTest, AddShapeCommandCanBeExecutedOnlyOnce)
{
	AddShapeCommand command(document, MakeRectangle({ 10, 10, 50, 50 }));
	command.Execute();

	EXPECT_THROW(command.Execute(), std::logic_error);
	EXPECT_EQ(document.GetShapeCount(), 1u);
}

TEST_F(CommandTest, AddShapeCommandRejectsEmptyShape)
{
	EXPECT_THROW(AddShapeCommand(document, nullptr), std::invalid_argument);
}

TEST_F(CommandTest, DeleteShapeCommandRemovesShape)
{
	const auto id = document.AddShape(MakeRectangle({ 0, 0, 100, 100 }));
	DeleteShapeCommand command(document, id);

	command.Execute();

	EXPECT_EQ(document.FindShape(id), nullptr);
}

TEST_F(CommandTest, DeleteShapeCommandIgnoresAlreadyRemovedShape)
{
	const auto id = document.AddShape(MakeRectangle({ 0, 0, 100, 100 }));
	DeleteShapeCommand command(document, id);
	command.Execute();

	EXPECT_NO_THROW(command.Execute());
}

TEST_F(CommandTest, ChangeShapeFrameCommandMovesAndResizesShape)
{
	const auto id = document.AddShape(MakeRectangle({ 0, 0, 100, 100 }));
	ChangeShapeFrameCommand command(document, id, { 20, 30, 40, 50 });

	command.Execute();

	EXPECT_EQ(document.FindShape(id)->GetFrame(), (Frame{ 20, 30, 40, 50 }));
}

TEST_F(CommandTest, ChangeShapeFrameCommandRejectsFrameOutsideCanvas)
{
	const auto id = document.AddShape(MakeRectangle({ 0, 0, 100, 100 }));
	ChangeShapeFrameCommand command(document, id, { 450, 0, 100, 100 });

	EXPECT_THROW(command.Execute(), std::invalid_argument);
	EXPECT_EQ(document.FindShape(id)->GetFrame(), (Frame{ 0, 0, 100, 100 }));
}
