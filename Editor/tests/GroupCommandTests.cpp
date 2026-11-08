//
// Created by Вадим Патрушев on 09.11.2026.
//

#include "Command/ChangeShapeIndexCommand/ChangeShapeIndexCommand.h"
#include "Command/UngroupShapeCommand/UngroupShapeCommand.h"
#include "Controller/EditorController.h"
#include "Mock/ShapeTypeVisitor.h"
#include "Model/Shapes/Ellipse.h"
#include "Model/Shapes/GroupShape.h"
#include "Model/Shapes/Rectangle.h"
#include <gtest/gtest.h>

class GroupCommandTest : public ::testing::Test
{
public:
	Document m_document{ 1000, 600 };
	EditorState m_state;
	CommandHistory m_history;
	EditorController m_controller{ m_document, m_state, m_history };
	int m_a = AddRectangle({ 0, 0, 50, 50 });
	int m_b = AddRectangle({ 100, 0, 50, 50 });
	int m_c = AddRectangle({ 200, 0, 50, 50 });
	int m_d = m_document.AddShape(std::make_unique<Ellipse>(Frame{ 300, 0, 50, 50 }, 0, 0));
	int m_e = AddRectangle({ 400, 0, 50, 50 });

	int AddRectangle(const Frame& frame)
	{
		return m_document.AddShape(std::make_unique<Rectangle>(frame, 0, 0));
	}

	std::vector<int> GetOrder() const
	{
		std::vector<int> order;
		for (size_t i = 0; i < m_document.GetShapeCount(); ++i)
		{
			order.push_back(m_document.GetShapeId(i));
		}
		return order;
	}

	int Group(const std::vector<int>& ids)
	{
		m_state.SetSelectedShapeIds(ids);
		m_controller.GroupSelectedShapes();
		return m_state.GetSelectedShapeIds().front();
	}

	std::vector<int> Ungroup(const int id)
	{
		m_state.SetSelectedShapeIds({ id });
		m_controller.UngroupSelectedShape();
		return m_state.GetSelectedShapeIds();
	}

	const GroupShape& GetGroup(const int id) const
	{
		return *m_document.FindShape(id)->GetGroup();
	}

	Frame GetFrame(const int id) const
	{
		return m_document.FindShape(id)->GetFrame();
	}

	void Drag(const Point from, const Point to)
	{
		m_controller.OnMouseDown(from);
		m_controller.OnMouseMove({ (from.m_x + to.m_x) / 2, (from.m_y + to.m_y) / 2 });
		m_controller.OnMouseMove(to);
		m_controller.OnMouseUp(to);
	}

	size_t CountUndoSteps()
	{
		size_t count = 0;
		while (m_history.CanUndo())
		{
			m_history.Undo();
			++count;
		}
		return count;
	}

	static void ExpectFrameNear(const Frame& actual, const Frame& expected)
	{
		EXPECT_NEAR(actual.m_left, expected.m_left, 1e-9);
		EXPECT_NEAR(actual.m_top, expected.m_top, 1e-9);
		EXPECT_NEAR(actual.m_width, expected.m_width, 1e-9);
		EXPECT_NEAR(actual.m_height, expected.m_height, 1e-9);
	}
};

TEST_F(GroupCommandTest, GroupOfNotAdjacentShapesIsPlacedOnTopAndKeepsTheirOrder)
{
	const int group = Group({ m_d, m_b });

	EXPECT_EQ(GetOrder(), (std::vector{ m_a, m_c, m_e, group }));
	const GroupShape& shape = GetGroup(group);
	ASSERT_EQ(shape.GetChildCount(), 2);
	EXPECT_EQ(shape.GetChild(0).GetFrame(), (Frame{ 100, 0, 50, 50 }));
	EXPECT_EQ(GetShapeType(shape.GetChild(1)), ShapeType::Ellipse);
	EXPECT_EQ(m_state.GetSelectedShapeIds(), std::vector{ group });
	EXPECT_EQ(CountUndoSteps(), 1);
}

TEST_F(GroupCommandTest, UndoGroupRestoresOriginalIndicesAndIds)
{
	Group({ m_b, m_d });

	m_controller.Undo();

	EXPECT_EQ(GetOrder(), (std::vector{ m_a, m_b, m_c, m_d, m_e }));
	EXPECT_EQ(m_state.GetSelectedShapeIds(), (std::vector{ m_b, m_d }));
	EXPECT_FALSE(m_history.CanUndo());
}

TEST_F(GroupCommandTest, RedoGroupRestoresSameGroup)
{
	const int group = Group({ m_b, m_d });
	m_controller.Undo();

	m_controller.Redo();

	EXPECT_EQ(GetOrder(), (std::vector{ m_a, m_c, m_e, group }));
	EXPECT_EQ(GetGroup(group).GetChildCount(), 2);
	EXPECT_EQ(m_state.GetSelectedShapeIds(), std::vector{ group });
}

TEST_F(GroupCommandTest, GroupOfLessThanTwoShapesDoesNothing)
{
	m_state.SetSelectedShapeIds({ m_b });
	m_controller.GroupSelectedShapes();
	m_state.SetSelectedShapeIds({});
	m_controller.GroupSelectedShapes();

	EXPECT_EQ(GetOrder(), (std::vector{ m_a, m_b, m_c, m_d, m_e }));
	EXPECT_FALSE(m_history.CanUndo());
}

TEST_F(GroupCommandTest, UngroupPlacesChildrenAtCurrentPlaceOfGroup)
{
	const int group = Group({ m_b, m_d });
	m_history.Execute(std::make_unique<ChangeShapeIndexCommand>(m_document, group, 1));

	const auto children = Ungroup(group);

	ASSERT_EQ(children.size(), 2);
	EXPECT_EQ(GetOrder(), (std::vector{ m_a, children[0], children[1], m_c, m_e }));
	EXPECT_EQ(GetFrame(children[0]), (Frame{ 100, 0, 50, 50 }));
	EXPECT_EQ(GetShapeType(*m_document.FindShape(children[1])), ShapeType::Ellipse);
}

TEST_F(GroupCommandTest, UngroupDiffersFromUndoGroup)
{
	const int group = Group({ m_b, m_d });

	const auto children = Ungroup(group);

	EXPECT_EQ(GetOrder(), (std::vector{ m_a, m_c, m_e, children[0], children[1] }));
	EXPECT_EQ(m_document.FindShape(group), nullptr);
}

TEST_F(GroupCommandTest, UndoAndRedoUngroup)
{
	const int group = Group({ m_b, m_d });
	const auto children = Ungroup(group);

	m_controller.Undo();
	EXPECT_EQ(GetOrder(), (std::vector{ m_a, m_c, m_e, group }));
	EXPECT_EQ(m_state.GetSelectedShapeIds(), std::vector{ group });

	m_controller.Redo();
	EXPECT_EQ(GetOrder(), (std::vector{ m_a, m_c, m_e, children[0], children[1] }));
	EXPECT_EQ(m_state.GetSelectedShapeIds(), children);
}

TEST_F(GroupCommandTest, UngroupKeepsNestedGroups)
{
	const int inner = Group({ m_b, m_d });
	const int outer = Group({ m_e, inner });

	const auto children = Ungroup(outer);

	ASSERT_EQ(children.size(), 2);
	EXPECT_EQ(m_document.FindShape(children[0])->GetGroup(), nullptr);
	ASSERT_NE(m_document.FindShape(children[1])->GetGroup(), nullptr);
	EXPECT_EQ(GetGroup(children[1]).GetChildCount(), 2);
}

TEST_F(GroupCommandTest, InvalidUngroupDoesNothing)
{
	Ungroup(m_a);
	const int group = Group({ m_b, m_d });
	m_state.SetSelectedShapeIds({ group, m_a });
	m_controller.UngroupSelectedShape();
	m_state.SetSelectedShapeIds({});
	m_controller.UngroupSelectedShape();

	EXPECT_EQ(GetOrder(), (std::vector{ m_a, m_c, m_e, group }));
	EXPECT_EQ(CountUndoSteps(), 1);
}

TEST_F(GroupCommandTest, UngroupCommandRejectsNotGroup)
{
	UngroupShapeCommand command(m_document, m_a);

	EXPECT_THROW(command.Execute(), std::invalid_argument);
	EXPECT_EQ(m_document.GetShapeCount(), 5);
}

TEST_F(GroupCommandTest, MoveGroupMovesAllChildrenAsOneOperation)
{
	const int group = Group({ m_b, m_d });

	Drag({ 125, 25 }, { 175, 75 });

	const GroupShape& shape = GetGroup(group);
	EXPECT_EQ(shape.GetChild(0).GetFrame(), (Frame{ 150, 50, 50, 50 }));
	EXPECT_EQ(shape.GetChild(1).GetFrame(), (Frame{ 350, 50, 50, 50 }));
	EXPECT_EQ(CountUndoSteps(), 2);
}

TEST_F(GroupCommandTest, MoveGroupShowsPreviewOfWholeGroup)
{
	const int group = Group({ m_b, m_d });

	m_controller.OnMouseDown({ 125, 25 });
	m_controller.OnMouseMove({ 175, 75 });

	EXPECT_EQ(m_state.GetPreviewFrame(group), (Frame{ 150, 50, 250, 50 }));
	EXPECT_EQ(GetFrame(group), (Frame{ 100, 0, 250, 50 }));
	m_controller.Cancel();
	EXPECT_EQ(CountUndoSteps(), 1);
}

TEST_F(GroupCommandTest, ResizeGroupOfDifferentTypesScalesSizesAndDistances)
{
	const int group = Group({ m_b, m_d });

	Drag({ 350, 50 }, { 600, 100 });

	const GroupShape& shape = GetGroup(group);
	ExpectFrameNear(shape.GetChild(0).GetFrame(), { 100, 0, 100, 100 });
	ExpectFrameNear(shape.GetChild(1).GetFrame(), { 500, 0, 100, 100 });
	EXPECT_EQ(CountUndoSteps(), 2);
}

TEST_F(GroupCommandTest, ResizeNestedGroupAndUndoRedo)
{
	const int inner = Group({ m_b, m_d });
	const int outer = Group({ m_a, inner });

	Drag({ 350, 50 }, { 700, 100 });

	const GroupShape& innerShape = *GetGroup(outer).GetChild(1).GetGroup();
	ExpectFrameNear(GetGroup(outer).GetChild(0).GetFrame(), { 0, 0, 100, 100 });
	ExpectFrameNear(innerShape.GetChild(0).GetFrame(), { 200, 0, 100, 100 });
	ExpectFrameNear(innerShape.GetChild(1).GetFrame(), { 600, 0, 100, 100 });

	m_controller.Undo();
	ExpectFrameNear(innerShape.GetChild(0).GetFrame(), { 100, 0, 50, 50 });
	ExpectFrameNear(innerShape.GetChild(1).GetFrame(), { 300, 0, 50, 50 });

	m_controller.Redo();
	ExpectFrameNear(innerShape.GetChild(1).GetFrame(), { 600, 0, 100, 100 });
}

TEST_F(GroupCommandTest, GroupMoveResizeUndoThreeTimesRedoThreeTimes)
{
	const int group = Group({ m_b, m_d });
	Drag({ 125, 25 }, { 125, 75 });
	Drag({ 350, 100 }, { 600, 150 });
	const auto finalFrame = GetFrame(group);

	m_controller.Undo();
	ExpectFrameNear(GetFrame(group), { 100, 50, 250, 50 });
	m_controller.Undo();
	ExpectFrameNear(GetFrame(group), { 100, 0, 250, 50 });
	m_controller.Undo();
	EXPECT_EQ(GetOrder(), (std::vector{ m_a, m_b, m_c, m_d, m_e }));
	EXPECT_EQ(GetFrame(m_b), (Frame{ 100, 0, 50, 50 }));

	m_controller.Redo();
	EXPECT_EQ(GetOrder(), (std::vector{ m_a, m_c, m_e, group }));
	m_controller.Redo();
	ExpectFrameNear(GetFrame(group), { 100, 50, 250, 50 });
	m_controller.Redo();
	ExpectFrameNear(GetFrame(group), finalFrame);
	EXPECT_FALSE(m_history.CanRedo());
}

TEST_F(GroupCommandTest, DeleteGroupWithContentAndUndoRedo)
{
	const int inner = Group({ m_b, m_d });
	const int outer = Group({ m_a, inner });
	m_history.Execute(std::make_unique<ChangeShapeIndexCommand>(m_document, outer, 0));
	m_state.SetSelectedShapeIds({ outer });

	m_controller.DeleteSelectedShapes();
	EXPECT_EQ(GetOrder(), (std::vector{ m_c, m_e }));
	EXPECT_TRUE(m_state.GetSelectedShapeIds().empty());

	m_controller.Undo();
	EXPECT_EQ(GetOrder(), (std::vector{ outer, m_c, m_e }));
	EXPECT_EQ(GetGroup(outer).GetChild(1).GetGroup()->GetChildCount(), 2);
	EXPECT_EQ(m_state.GetSelectedShapeIds(), std::vector{ outer });

	m_controller.Redo();
	EXPECT_EQ(GetOrder(), (std::vector{ m_c, m_e }));
}

TEST_F(GroupCommandTest, ClickOnGroupWithoutMoveCreatesNoCommand)
{
	Group({ m_b, m_d });

	Drag({ 125, 25 }, { 125, 25 });

	EXPECT_EQ(CountUndoSteps(), 1);
}

TEST_F(GroupCommandTest, ClickOnChildOfNotSelectedGroupSelectsWholeGroup)
{
	const int group = Group({ m_b, m_d });
	m_state.SetSelectedShapeIds({});

	m_controller.OnMouseDown({ 325, 25 });
	m_controller.OnMouseUp({ 325, 25 });

	EXPECT_EQ(m_state.GetSelectedShapeIds(), std::vector{ group });
}

TEST_F(GroupCommandTest, ClickBetweenChildrenOfGroupSelectsNothing)
{
	Group({ m_b, m_d });
	m_state.SetSelectedShapeIds({});

	m_controller.OnMouseDown({ 175, 25 });
	m_controller.OnMouseUp({ 175, 25 });

	EXPECT_TRUE(m_state.GetSelectedShapeIds().empty());
}

TEST_F(GroupCommandTest, CommandsOnShapesBeforeGroupStayValidAfterUndoGroup)
{
	m_state.SetSelectedShapeIds({ m_b });
	Drag({ 125, 25 }, { 125, 125 });
	Group({ m_b, m_d });

	m_controller.Undo();
	m_controller.Undo();

	EXPECT_EQ(GetFrame(m_b), (Frame{ 100, 0, 50, 50 }));
	m_controller.Redo();
	m_controller.Redo();
	EXPECT_EQ(GetGroup(m_state.GetSelectedShapeIds().front()).GetChild(0).GetFrame(), (Frame{ 100, 100, 50, 50 }));
}

TEST_F(GroupCommandTest, GroupInOneWindowRemovesGroupedShapesFromOtherWindowSelection)
{
	EditorState otherState;
	EditorController other{ m_document, otherState, m_history };
	otherState.SetSelectedShapeIds({ m_a, m_b });

	Group({ m_b, m_d });

	EXPECT_EQ(otherState.GetSelectedShapeIds(), std::vector{ m_a });
}
