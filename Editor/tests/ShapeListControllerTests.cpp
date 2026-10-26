//
// Created by Вадим Патрушев on 26.10.2026.
//

#include "Controller/ShapeListController.h"
#include "Mock/MockDocumentObserver.h"
#include "Model/Shapes/Rectangle.h"
#include <gtest/gtest.h>

class ShapeListControllerTest : public ::testing::Test
{
public:
	Document m_document{ 1000, 600 };
	EditorState m_state;
	CommandHistory m_history;
	ShapeListController m_controller{ m_document, m_state, m_history };
	MockDocumentObserver m_documentObserver;
	int m_bottom = AddRectangle();
	int m_middle = AddRectangle();
	int m_top = AddRectangle();

	void TearDown() override
	{
		m_document.RemoveObserver(m_documentObserver);
	}

	int AddRectangle()
	{
		return m_document.AddShape(std::make_unique<Rectangle>(Frame{ 0, 0, 100, 100 }, 0));
	}

	void DragRow(const size_t row, const size_t slot)
	{
		m_controller.OnMouseDown(row);
		m_controller.OnMouseMove(slot);
		m_controller.OnMouseUp(slot);
	}

	std::vector<int> GetIds() const
	{
		std::vector<int> ids;
		for (size_t i = 0; i < m_document.GetShapeCount(); ++i)
		{
			ids.push_back(m_document.GetShapeId(i));
		}
		return ids;
	}
};

TEST_F(ShapeListControllerTest, ClickOnFirstRowSelectsTopShape)
{
	m_controller.OnMouseDown(0);

	EXPECT_EQ(m_state.GetSelectedShapeId(), m_top);
}

TEST_F(ShapeListControllerTest, ClickOnLastRowSelectsBottomShape)
{
	m_controller.OnMouseDown(2);

	EXPECT_EQ(m_state.GetSelectedShapeId(), m_bottom);
}

TEST_F(ShapeListControllerTest, ClickBelowRowsClearsSelection)
{
	m_state.SetSelectedShapeId(m_top);

	m_controller.OnMouseDown(std::nullopt);

	EXPECT_EQ(m_state.GetSelectedShapeId(), std::nullopt);
}

TEST_F(ShapeListControllerTest, ClickWithoutDragDoesNotChangeDocument)
{
	m_document.AddObserver(m_documentObserver);
	EXPECT_CALL(m_documentObserver, DocumentUpdate).Times(0);

	m_controller.OnMouseDown(1);
	m_controller.OnMouseUp(1);
}

TEST_F(ShapeListControllerTest, DragTopRowToBottomMovesShapeToBottomLayer)
{
	DragRow(0, 3);

	EXPECT_EQ(GetIds(), (std::vector{ m_top, m_bottom, m_middle }));
}

TEST_F(ShapeListControllerTest, DragBottomRowToTopMovesShapeToTopLayer)
{
	DragRow(2, 0);

	EXPECT_EQ(GetIds(), (std::vector{ m_middle, m_top, m_bottom }));
}

TEST_F(ShapeListControllerTest, DragRowOneStepDownSwapsNeighbours)
{
	DragRow(0, 2);

	EXPECT_EQ(GetIds(), (std::vector{ m_bottom, m_top, m_middle }));
}

TEST_F(ShapeListControllerTest, DropNextToItselfDoesNotChangeDocument)
{
	m_document.AddObserver(m_documentObserver);
	EXPECT_CALL(m_documentObserver, DocumentUpdate).Times(0);

	DragRow(1, 1);
	DragRow(1, 2);
}

TEST_F(ShapeListControllerTest, DragKeepsDraggedShapeSelected)
{
	DragRow(2, 0);

	EXPECT_EQ(m_state.GetSelectedShapeId(), m_bottom);
}

TEST_F(ShapeListControllerTest, DropSlotIsShownOnlyWhenOrderWouldChange)
{
	m_controller.OnMouseDown(1);

	m_controller.OnMouseMove(2);
	EXPECT_EQ(m_controller.GetDropSlot(), std::nullopt);

	m_controller.OnMouseMove(3);
	EXPECT_EQ(m_controller.GetDropSlot(), 3u);
}

TEST_F(ShapeListControllerTest, CancelDropsDragAndIgnoresMouseUp)
{
	m_controller.OnMouseDown(0);
	m_controller.OnMouseMove(3);

	m_controller.Cancel();
	m_controller.OnMouseUp(3);

	EXPECT_EQ(m_controller.GetDropSlot(), std::nullopt);
	EXPECT_EQ(GetIds(), (std::vector{ m_bottom, m_middle, m_top }));
}

TEST_F(ShapeListControllerTest, MouseUpAfterDraggedShapeWasDeletedIsIgnored)
{
	m_controller.OnMouseDown(0);
	m_document.RemoveShape(m_top);

	EXPECT_NO_THROW(m_controller.OnMouseUp(2));
	EXPECT_EQ(GetIds(), (std::vector{ m_bottom, m_middle }));
}

TEST_F(ShapeListControllerTest, UndoRestoresOrderAfterDrag)
{
	DragRow(0, 3);

	m_history.Undo();

	EXPECT_EQ(GetIds(), (std::vector{ m_bottom, m_middle, m_top }));
}

TEST_F(ShapeListControllerTest, DropNextToItselfDoesNotAddHistory)
{
	DragRow(1, 2);

	EXPECT_FALSE(m_history.CanUndo());
}
