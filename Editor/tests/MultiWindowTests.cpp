//
// Created by Вадим Патрушев on 08.11.2026.
//

#include "Application/AppDocument.h"
#include "Controller/EditorController.h"
#include "Controller/ShapeListController.h"
#include "Model/Shapes/Rectangle.h"
#include <gtest/gtest.h>

class MultiWindowTest : public ::testing::Test
{
public:
	AppDocument m_appDocument{ 1000, 600, "Документ 1" };
	Document& m_document = m_appDocument.GetDocument();
	CommandHistory& m_history = m_appDocument.GetCommandHistory();
	EditorState m_firstState;
	EditorController m_first{ m_document, m_firstState, m_history };
	EditorState m_secondState;
	EditorController m_second{ m_document, m_secondState, m_history };
	ShapeListController m_secondList{ m_document, m_secondState, m_history };

	int AddRectangle(const Frame& frame)
	{
		return m_document.AddShape(std::make_unique<Rectangle>(frame, 0, 0));
	}
};

TEST_F(MultiWindowTest, WindowsHaveIndependentSelection)
{
	const auto first = AddRectangle({ 0, 0, 100, 100 });
	const auto second = AddRectangle({ 200, 200, 100, 100 });

	m_first.OnMouseDown({ 50, 50 });
	m_first.OnMouseUp({ 50, 50 });
	m_second.OnMouseDown({ 250, 250 });
	m_second.OnMouseUp({ 250, 250 });

	EXPECT_EQ(m_firstState.GetSelectedShapeIds(), std::vector{ first });
	EXPECT_EQ(m_secondState.GetSelectedShapeIds(), std::vector{ second });
}

TEST_F(MultiWindowTest, ChangeInOneWindowIsVisibleInOther)
{
	m_first.AddShape(ShapeType::Rectangle);

	EXPECT_EQ(m_document.GetShapeCount(), 1);
	EXPECT_TRUE(m_secondState.GetSelectedShapeIds().empty());
}

TEST_F(MultiWindowTest, DeleteInOneWindowRemovesShapeFromOtherSelection)
{
	const auto first = AddRectangle({ 0, 0, 100, 100 });
	const auto second = AddRectangle({ 200, 200, 100, 100 });
	m_firstState.SetSelectedShapeIds({ first });
	m_secondState.SetSelectedShapeIds({ first, second });

	m_first.DeleteSelectedShapes();

	EXPECT_EQ(m_secondState.GetSelectedShapeIds(), std::vector{ second });
}

TEST_F(MultiWindowTest, DeleteOfOtherShapeKeepsSelection)
{
	const auto first = AddRectangle({ 0, 0, 100, 100 });
	const auto second = AddRectangle({ 200, 200, 100, 100 });
	m_firstState.SetSelectedShapeIds({ first });
	m_secondState.SetSelectedShapeIds({ second });

	m_first.DeleteSelectedShapes();

	EXPECT_EQ(m_secondState.GetSelectedShapeIds(), std::vector{ second });
}

TEST_F(MultiWindowTest, UndoInOtherWindowChangesSharedDocument)
{
	m_first.AddShape(ShapeType::Rectangle);

	m_second.Undo();

	EXPECT_EQ(m_document.GetShapeCount(), 0);
	EXPECT_TRUE(m_firstState.GetSelectedShapeIds().empty());
	EXPECT_TRUE(m_history.CanRedo());
}

TEST_F(MultiWindowTest, UndoSelectsShapesOnlyInActiveWindow)
{
	const auto first = AddRectangle({ 0, 0, 100, 100 });
	const auto second = AddRectangle({ 200, 200, 100, 100 });
	m_firstState.SetSelectedShapeIds({ first });
	m_first.DeleteSelectedShapes();
	m_secondState.SetSelectedShapeIds({ second });

	m_second.Undo();

	EXPECT_EQ(m_secondState.GetSelectedShapeIds(), std::vector{ first });
	EXPECT_TRUE(m_firstState.GetSelectedShapeIds().empty());
}

TEST_F(MultiWindowTest, DragIsCancelledWhenShapeIsDeletedInOtherWindow)
{
	const auto id = AddRectangle({ 0, 0, 100, 100 });
	m_firstState.SetSelectedShapeIds({ id });
	m_secondState.SetSelectedShapeIds({ id });
	m_second.OnMouseDown({ 50, 50 });
	m_second.OnMouseMove({ 150, 150 });
	ASSERT_FALSE(m_secondState.GetPreviewFrames().empty());

	m_first.DeleteSelectedShapes();

	EXPECT_TRUE(m_secondState.GetPreviewFrames().empty());
	EXPECT_NO_THROW(m_second.OnMouseUp({ 200, 200 }));
	m_second.Undo();
	EXPECT_FALSE(m_history.CanUndo());
	ASSERT_NE(m_document.FindShape(id), nullptr);
	EXPECT_EQ(m_document.FindShape(id)->GetFrame(), (Frame{ 0, 0, 100, 100 }));
}

TEST_F(MultiWindowTest, DragInOtherWindowContinuesWhenAnotherShapeIsDeleted)
{
	const auto first = AddRectangle({ 0, 0, 100, 100 });
	const auto second = AddRectangle({ 200, 200, 100, 100 });
	m_firstState.SetSelectedShapeIds({ first });
	m_secondState.SetSelectedShapeIds({ second });
	m_second.OnMouseDown({ 250, 250 });
	m_second.OnMouseMove({ 300, 300 });

	m_first.DeleteSelectedShapes();
	m_second.OnMouseUp({ 300, 300 });

	EXPECT_EQ(m_document.FindShape(second)->GetFrame(), (Frame{ 250, 250, 100, 100 }));
}

TEST_F(MultiWindowTest, ShapeListDragOfDeletedShapeDoesNotChangeHistory)
{
	AddRectangle({ 0, 0, 100, 100 });
	const auto id = AddRectangle({ 200, 200, 100, 100 });
	m_secondList.OnMouseDown(0);
	m_secondList.OnMouseMove(2);
	m_firstState.SetSelectedShapeIds({ id });

	m_first.DeleteSelectedShapes();
	m_secondList.OnMouseUp(2);

	m_history.Undo();
	EXPECT_FALSE(m_history.CanUndo());
}

TEST_F(MultiWindowTest, WindowsHaveIndependentCurrentTime)
{
	m_firstState.SetCurrentTime(2);
	m_secondState.SetCurrentTime(7);

	EXPECT_EQ(m_firstState.GetCurrentTime(), 2);
	EXPECT_EQ(m_secondState.GetCurrentTime(), 7);
}

TEST_F(MultiWindowTest, ShapeVisibilityDependsOnWindowTime)
{
	const auto id = AddRectangle({ 0, 0, 100, 100 });
	m_document.SetShapeTimeInterval(id, { 0, 5 });
	m_secondState.SetCurrentTime(6);

	m_first.OnMouseDown({ 50, 50 });
	m_first.OnMouseUp({ 50, 50 });
	m_second.OnMouseDown({ 50, 50 });
	m_second.OnMouseUp({ 50, 50 });

	EXPECT_EQ(m_firstState.GetSelectedShapeIds(), std::vector{ id });
	EXPECT_TRUE(m_secondState.GetSelectedShapeIds().empty());
}
