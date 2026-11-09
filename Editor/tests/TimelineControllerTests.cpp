//
// Created by Вадим Патрушев on 08.11.2026.
//

#include "Controller/EditorController.h"
#include "Controller/ShapeDefaults.h"
#include "Controller/TimelineController.h"
#include "Model/Shapes/Rectangle.h"
#include <gtest/gtest.h>

class TimelineControllerTest : public ::testing::Test
{
public:
	Document m_document{ 1000, 600 };
	EditorState m_state;
	CommandHistory m_history;
	TimelineController m_controller{ m_document, m_state, m_history };
	int m_bottom = AddRectangle({ 0, 4 });
	int m_middle = AddRectangle({ 2, 6 });
	int m_top = AddRectangle({ 5, 9 });

	int AddRectangle(const TimeInterval& interval)
	{
		auto shape = std::make_unique<Rectangle>(Frame{ 0, 0, 100, 100 }, 0, 0);
		shape->SetTimeInterval(interval);
		return m_document.AddShape(std::move(shape));
	}

	TimeInterval GetInterval(const int id) const
	{
		return m_document.FindShape(id)->GetTimeInterval();
	}

	std::vector<int> GetLayerOrder() const
	{
		std::vector<int> order;
		for (size_t i = 0; i < m_document.GetShapeCount(); ++i)
		{
			order.push_back(m_document.GetShapeId(i));
		}
		return order;
	}

	void DragTime(const size_t row, const double from, const double to, const TimelineDragMode mode)
	{
		m_controller.OnMouseDown(row, from, mode);
		m_controller.OnMouseMove(row, (from + to) / 2);
		m_controller.OnMouseMove(row, to);
		m_controller.OnMouseUp(row, to);
	}

	void DragLayers(const size_t row, const size_t slot)
	{
		m_controller.OnMouseDown(row, 0, TimelineDragMode::MoveLayers);
		m_controller.OnMouseMove(slot, 0);
		m_controller.OnMouseUp(slot, 0);
	}
};

TEST_F(TimelineControllerTest, ClickOnRowSelectsShape)
{
	m_controller.OnMouseDown(0, 0, TimelineDragMode::None);

	EXPECT_EQ(m_state.GetSelectedShapeIds(), std::vector{ m_top });
}

TEST_F(TimelineControllerTest, ToggleClickAddsAndRemoves)
{
	m_controller.OnMouseDown(0, 0, TimelineDragMode::None);
	m_controller.OnMouseDown(2, 0, TimelineDragMode::None, true);
	EXPECT_EQ(m_state.GetSelectedShapeIds(), (std::vector{ m_top, m_bottom }));

	m_controller.OnMouseDown(0, 0, TimelineDragMode::None, true);
	EXPECT_EQ(m_state.GetSelectedShapeIds(), std::vector{ m_bottom });
}

TEST_F(TimelineControllerTest, ClickBelowRowsClearsSelection)
{
	m_state.SetSelectedShapeIds({ m_top });

	m_controller.OnMouseDown(std::nullopt, 0, TimelineDragMode::None);

	EXPECT_TRUE(m_state.GetSelectedShapeIds().empty());
}

TEST_F(TimelineControllerTest, ScrubAddsNoHistory)
{
	m_controller.OnMouseDown(std::nullopt, 1, TimelineDragMode::Scrub);
	m_controller.OnMouseMove(0, 3);
	EXPECT_EQ(m_state.GetCurrentTime(), 3);

	m_controller.OnMouseUp(0, 4);

	EXPECT_EQ(m_state.GetCurrentTime(), 4);
	EXPECT_FALSE(m_history.CanUndo());
}

TEST_F(TimelineControllerTest, CurrentTimeIsClamped)
{
	m_controller.SetCurrentTime(-5);
	EXPECT_EQ(m_state.GetCurrentTime(), 0);

	m_controller.SetCurrentTime(100);
	EXPECT_EQ(m_state.GetCurrentTime(), m_document.GetDuration());
}

TEST_F(TimelineControllerTest, MoveTimeShowsPreview)
{
	m_controller.OnMouseDown(1, 3, TimelineDragMode::MoveTime);
	m_controller.OnMouseMove(1, 4);

	EXPECT_EQ(m_state.GetPreviewTimeInterval(m_middle), (TimeInterval{ 3, 7 }));
	EXPECT_EQ(GetInterval(m_middle), (TimeInterval{ 2, 6 }));
	EXPECT_FALSE(m_history.CanUndo());
}

TEST_F(TimelineControllerTest, MoveTimeIsOneOperation)
{
	DragTime(1, 3, 4, TimelineDragMode::MoveTime);

	EXPECT_EQ(GetInterval(m_middle), (TimeInterval{ 3, 7 }));
	EXPECT_TRUE(m_state.GetPreviewTimeIntervals().empty());

	m_history.Undo();
	EXPECT_EQ(GetInterval(m_middle), (TimeInterval{ 2, 6 }));
	EXPECT_FALSE(m_history.CanUndo());

	m_history.Redo();
	EXPECT_EQ(GetInterval(m_middle), (TimeInterval{ 3, 7 }));
}

TEST_F(TimelineControllerTest, MoveTimeOfSelectedStopsAtEnd)
{
	m_state.SetSelectedShapeIds({ m_bottom, m_top });

	DragTime(0, 6, 9, TimelineDragMode::MoveTime);

	EXPECT_EQ(GetInterval(m_bottom), (TimeInterval{ 1, 5 }));
	EXPECT_EQ(GetInterval(m_top), (TimeInterval{ 6, 10 }));
}

TEST_F(TimelineControllerTest, ResizeStart)
{
	DragTime(1, 2, 1, TimelineDragMode::ResizeStart);

	EXPECT_EQ(GetInterval(m_middle), (TimeInterval{ 1, 6 }));
}

TEST_F(TimelineControllerTest, ResizeEnd)
{
	DragTime(1, 6, 8, TimelineDragMode::ResizeEnd);

	EXPECT_EQ(GetInterval(m_middle), (TimeInterval{ 2, 8 }));
}

TEST_F(TimelineControllerTest, ResizeKeepsMinDuration)
{
	DragTime(1, 2, 20, TimelineDragMode::ResizeStart);

	EXPECT_NEAR(GetInterval(m_middle).GetDuration(), ShapeDefaults::MIN_DURATION, 1e-9);
	EXPECT_EQ(GetInterval(m_middle).m_end, 6);
}

TEST_F(TimelineControllerTest, MoveLayerToTop)
{
	DragLayers(2, 0);

	EXPECT_EQ(GetLayerOrder(), (std::vector{ m_middle, m_top, m_bottom }));
	m_history.Undo();
	EXPECT_EQ(GetLayerOrder(), (std::vector{ m_bottom, m_middle, m_top }));
	EXPECT_FALSE(m_history.CanUndo());
}

TEST_F(TimelineControllerTest, MoveSeveralLayers)
{
	const auto fourth = AddRectangle({ 0, 1 });
	m_state.SetSelectedShapeIds({ fourth, m_middle });

	DragLayers(0, 4);

	EXPECT_EQ(GetLayerOrder(), (std::vector{ m_middle, fourth, m_bottom, m_top }));
	m_history.Undo();
	EXPECT_EQ(GetLayerOrder(), (std::vector{ m_bottom, m_middle, m_top, fourth }));
	EXPECT_FALSE(m_history.CanUndo());
}

TEST_F(TimelineControllerTest, DropAtSamePlaceAddsNoHistory)
{
	DragLayers(1, 1);

	EXPECT_EQ(GetLayerOrder(), (std::vector{ m_bottom, m_middle, m_top }));
	EXPECT_FALSE(m_history.CanUndo());
}

TEST_F(TimelineControllerTest, UndoSelectsChangedShape)
{
	EditorController editor{ m_document, m_state, m_history };
	DragTime(1, 3, 4, TimelineDragMode::MoveTime);
	m_state.SetSelectedShapeIds({});

	editor.Undo();

	EXPECT_EQ(m_state.GetSelectedShapeIds(), std::vector{ m_middle });
}

TEST_F(TimelineControllerTest, DragStopsWhenShapeIsDeleted)
{
	m_controller.OnMouseDown(1, 3, TimelineDragMode::MoveTime);
	m_controller.OnMouseMove(1, 5);

	m_document.DeleteShape(m_middle);

	EXPECT_EQ(m_controller.GetDragMode(), TimelineDragMode::None);
	EXPECT_TRUE(m_state.GetPreviewTimeIntervals().empty());
	EXPECT_NO_THROW(m_controller.OnMouseUp(1, 5));
	EXPECT_FALSE(m_history.CanUndo());
}
