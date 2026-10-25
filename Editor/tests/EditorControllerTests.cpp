//
// Created by Вадим Патрушев on 15.10.2026.
//

#include "Controller/EditorController.h"
#include "Controller/ShapeDefaults.h"
#include "Mock/MockDocumentObserver.h"
#include "Mock/ResizeCase.h"
#include "Model/Shapes/Ellipse.h"
#include "Model/Shapes/Rectangle.h"
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>

class EditorControllerTest : public ::testing::Test
{
public:
	Document m_document{ 1000, 600 };
	EditorState m_state;
	EditorController m_controller{ m_document, m_state };
	MockDocumentObserver m_documentObserver;
	std::filesystem::path m_path = std::filesystem::temp_directory_path() / "test.draw";

	void TearDown() override
	{
		m_document.RemoveObserver(m_documentObserver);
		std::filesystem::remove(m_path);
	}

	int AddRectangle(const Frame& frame)
	{
		return m_document.AddShape(std::make_unique<Rectangle>(frame, 0));
	}

	int AddSelectedRectangle(const Frame& frame)
	{
		const auto id = AddRectangle(frame);
		m_state.SetSelectedShapeId(id);
		return id;
	}

	Frame GetFrame(const int id) const
	{
		return m_document.FindShape(id)->GetFrame();
	}

	void Click(const Point point)
	{
		m_controller.OnMouseDown(point);
		m_controller.OnMouseUp(point);
	}

	void Drag(const Point from, const Point to)
	{
		m_controller.OnMouseDown(from);
		m_controller.OnMouseMove({ (from.m_x + to.m_x) / 2, (from.m_y + to.m_y) / 2 });
		m_controller.OnMouseMove(to);
		m_controller.OnMouseUp(to);
	}

	void WriteFile(const std::string& content) const
	{
		std::ofstream file(m_path);
		file << content;
	}
};

TEST_F(EditorControllerTest, AddShapePlacesItOnTopWithRequestedType)
{
	AddRectangle({ 0, 0, 100, 100 });

	m_controller.AddShape(ShapeType::Ellipse);

	ASSERT_EQ(m_document.GetShapeCount(), 2u);
	EXPECT_EQ(m_document.GetShape(1).GetType(), ShapeType::Ellipse);
}

TEST_F(EditorControllerTest, AddShapePlacesItInCanvasCenter)
{
	m_controller.AddShape(ShapeType::Triangle);

	EXPECT_EQ(m_document.GetShape(0).GetFrame().GetCenter(), (Point{ 500, 300 }));
}

TEST_F(EditorControllerTest, AddShapeSelectsIt)
{
	m_controller.AddShape(ShapeType::Rectangle);

	EXPECT_EQ(m_state.GetSelectedShapeId(), m_document.GetShapeId(0));
}

TEST_F(EditorControllerTest, ClickSelectsTopShape)
{
	AddRectangle({ 0, 0, 200, 200 });
	const auto top = AddRectangle({ 100, 100, 200, 200 });

	Click({ 150, 150 });

	EXPECT_EQ(m_state.GetSelectedShapeId(), top);
}

TEST_F(EditorControllerTest, ClickOnUncoveredPartSelectsLowerShape)
{
	const auto bottom = AddRectangle({ 0, 0, 200, 200 });
	AddRectangle({ 100, 100, 200, 200 });

	Click({ 50, 50 });

	EXPECT_EQ(m_state.GetSelectedShapeId(), bottom);
}

TEST_F(EditorControllerTest, ClickOnEmptyAreaClearsSelection)
{
	AddSelectedRectangle({ 0, 0, 200, 200 });

	Click({ 800, 500 });

	EXPECT_EQ(m_state.GetSelectedShapeId(), std::nullopt);
}

TEST_F(EditorControllerTest, ClickInEllipseBoundingBoxCornerDoesNotSelect)
{
	m_document.AddShape(std::make_unique<Ellipse>(Frame{ 100, 100, 200, 100 }, 0));

	Click({ 105, 105 });

	EXPECT_EQ(m_state.GetSelectedShapeId(), std::nullopt);
}

TEST_F(EditorControllerTest, DragKeepsOffsetBetweenCursorAndShape)
{
	const auto id = AddRectangle({ 100, 100, 200, 100 });

	Drag({ 110, 120 }, { 410, 320 });

	EXPECT_EQ(GetFrame(id), (Frame{ 400, 300, 200, 100 }));
}

TEST_F(EditorControllerTest, DragSelectsDraggedShape)
{
	const auto id = AddRectangle({ 100, 100, 200, 100 });

	Drag({ 110, 120 }, { 410, 320 });

	EXPECT_EQ(m_state.GetSelectedShapeId(), id);
}

TEST_F(EditorControllerTest, DragIsLimitedByTopLeftOfCanvas)
{
	const auto id = AddRectangle({ 100, 100, 200, 100 });

	Drag({ 150, 150 }, { -500, -500 });

	EXPECT_EQ(GetFrame(id), (Frame{ 0, 0, 200, 100 }));
}

TEST_F(EditorControllerTest, DragIsLimitedByBottomRightOfCanvas)
{
	const auto id = AddRectangle({ 100, 100, 200, 100 });

	Drag({ 150, 150 }, { 5000, 5000 });

	EXPECT_EQ(GetFrame(id), (Frame{ 800, 500, 200, 100 }));
}

class EditorControllerResizeTest : public EditorControllerTest
	, public ::testing::WithParamInterface<ResizeCase>
{
};

TEST_P(EditorControllerResizeTest, ResizeByHandleMovesOnlyItsSides)
{
	const auto& [name, handle, offset, expected] = GetParam();
	const auto id = AddSelectedRectangle({ 100, 100, 200, 100 });

	Drag(handle, { handle.m_x + offset.m_x, handle.m_y + offset.m_y });

	EXPECT_EQ(GetFrame(id), expected);
}

INSTANTIATE_TEST_SUITE_P(EditorControllerTest, EditorControllerResizeTest,
	::testing::Values(
		ResizeCase{ "TopLeft", { 100, 100 }, { -10, -20 }, { 90, 80, 210, 120 } },
		ResizeCase{ "Top", { 200, 100 }, { 10, -20 }, { 100, 80, 200, 120 } },
		ResizeCase{ "TopRight", { 300, 100 }, { 10, -20 }, { 100, 80, 210, 120 } },
		ResizeCase{ "Right", { 300, 150 }, { 10, 20 }, { 100, 100, 210, 100 } },
		ResizeCase{ "BottomRight", { 300, 200 }, { 10, 20 }, { 100, 100, 210, 120 } },
		ResizeCase{ "Bottom", { 200, 200 }, { 10, 20 }, { 100, 100, 200, 120 } },
		ResizeCase{ "BottomLeft", { 100, 200 }, { -10, 20 }, { 90, 100, 210, 120 } },
		ResizeCase{ "Left", { 100, 150 }, { -10, 20 }, { 90, 100, 210, 100 } }),
	[](const ::testing::TestParamInfo<ResizeCase>& info) { return info.param.m_name; });

TEST_F(EditorControllerTest, ResizeByBottomRightIsLimitedByMinimumSize)
{
	const auto id = AddSelectedRectangle({ 100, 100, 200, 100 });

	Drag({ 300, 200 }, { 0, 0 });

	EXPECT_EQ(GetFrame(id), (Frame{ 100, 100, ShapeDefaults::MIN_SIZE, ShapeDefaults::MIN_SIZE }));
}

TEST_F(EditorControllerTest, ResizeByTopLeftIsLimitedByMinimumSize)
{
	const auto id = AddSelectedRectangle({ 100, 100, 200, 100 });

	Drag({ 100, 100 }, { 900, 900 });

	EXPECT_EQ(GetFrame(id), (Frame{ 290, 190, ShapeDefaults::MIN_SIZE, ShapeDefaults::MIN_SIZE }));
}

TEST_F(EditorControllerTest, ResizeByBottomRightIsLimitedByCanvas)
{
	const auto id = AddSelectedRectangle({ 100, 100, 200, 100 });

	Drag({ 300, 200 }, { 5000, 5000 });

	EXPECT_EQ(GetFrame(id), (Frame{ 100, 100, 900, 500 }));
}

TEST_F(EditorControllerTest, ResizeByTopLeftIsLimitedByCanvas)
{
	const auto id = AddSelectedRectangle({ 100, 100, 200, 100 });

	Drag({ 100, 100 }, { -100, -100 });

	EXPECT_EQ(GetFrame(id), (Frame{ 0, 0, 300, 200 }));
}

TEST_F(EditorControllerTest, HandleOfCoveredSelectedShapeHasPriority)
{
	AddSelectedRectangle({ 100, 100, 200, 100 });
	AddRectangle({ 50, 50, 400, 300 });

	EXPECT_EQ(m_controller.HitTest({ 300, 200 }).m_area, HitArea::ResizeHandle);
}

TEST_F(EditorControllerTest, BorderOfCoveredSelectedShapeHasPriority)
{
	AddSelectedRectangle({ 100, 100, 200, 100 });
	AddRectangle({ 50, 50, 400, 300 });

	EXPECT_EQ(m_controller.HitTest({ 150, 100 }).m_area, HitArea::SelectionBorder);
}

TEST_F(EditorControllerTest, CoveredSelectedShapeCanBeResized)
{
	const auto covered = AddSelectedRectangle({ 100, 100, 200, 100 });
	AddRectangle({ 50, 50, 400, 300 });

	Drag({ 300, 200 }, { 350, 250 });

	EXPECT_EQ(GetFrame(covered), (Frame{ 100, 100, 250, 150 }));
	EXPECT_EQ(m_state.GetSelectedShapeId(), covered);
}

TEST_F(EditorControllerTest, CoveredSelectedShapeCanBeMovedByBorder)
{
	const auto covered = AddSelectedRectangle({ 100, 100, 200, 100 });
	AddRectangle({ 50, 50, 400, 300 });

	Drag({ 150, 100 }, { 160, 110 });

	EXPECT_EQ(GetFrame(covered), (Frame{ 110, 110, 200, 100 }));
	EXPECT_EQ(m_state.GetSelectedShapeId(), covered);
}

TEST_F(EditorControllerTest, CursorIsArrowOverUnselectedShape)
{
	AddRectangle({ 100, 100, 200, 100 });

	EXPECT_EQ(m_controller.GetCursorType({ 100, 100 }), CursorType::Arrow);
}

TEST_F(EditorControllerTest, CursorIsDiagonalOverCornerHandles)
{
	AddSelectedRectangle({ 100, 100, 200, 100 });

	EXPECT_EQ(m_controller.GetCursorType({ 100, 100 }), CursorType::SizeTopLeftBottomRight);
	EXPECT_EQ(m_controller.GetCursorType({ 300, 100 }), CursorType::SizeBottomLeftTopRight);
}

TEST_F(EditorControllerTest, CursorIsStraightOverSideHandles)
{
	AddSelectedRectangle({ 100, 100, 200, 100 });

	EXPECT_EQ(m_controller.GetCursorType({ 200, 100 }), CursorType::SizeVertical);
	EXPECT_EQ(m_controller.GetCursorType({ 300, 150 }), CursorType::SizeHorizontal);
}

TEST_F(EditorControllerTest, CursorIsMoveOverSelectionBorder)
{
	AddSelectedRectangle({ 100, 100, 200, 100 });

	EXPECT_EQ(m_controller.GetCursorType({ 150, 100 }), CursorType::Move);
}

TEST_F(EditorControllerTest, CursorIsArrowOverEmptyArea)
{
	AddSelectedRectangle({ 100, 100, 200, 100 });

	EXPECT_EQ(m_controller.GetCursorType({ 600, 400 }), CursorType::Arrow);
}

TEST_F(EditorControllerTest, CursorKeepsResizeTypeDuringResize)
{
	AddSelectedRectangle({ 100, 100, 200, 100 });

	m_controller.OnMouseDown({ 300, 150 });
	m_controller.OnMouseMove({ 600, 400 });

	EXPECT_EQ(m_controller.GetCursorType({ 600, 400 }), CursorType::SizeHorizontal);
}

TEST_F(EditorControllerTest, CursorKeepsMoveTypeDuringMove)
{
	AddRectangle({ 100, 100, 200, 100 });

	m_controller.OnMouseDown({ 150, 150 });
	m_controller.OnMouseMove({ 900, 550 });

	EXPECT_EQ(m_controller.GetCursorType({ 900, 550 }), CursorType::Move);
}

TEST_F(EditorControllerTest, DeleteRemovesOnlySelectedShape)
{
	AddRectangle({ 0, 0, 100, 100 });
	const auto id = AddSelectedRectangle({ 200, 200, 100, 100 });

	m_controller.DeleteSelectedShape();

	EXPECT_EQ(m_document.GetShapeCount(), 1u);
	EXPECT_EQ(m_document.FindShape(id), nullptr);
}

TEST_F(EditorControllerTest, DeleteClearsSelection)
{
	AddSelectedRectangle({ 200, 200, 100, 100 });

	m_controller.DeleteSelectedShape();

	EXPECT_EQ(m_state.GetSelectedShapeId(), std::nullopt);
}

TEST_F(EditorControllerTest, DeleteWithoutSelectionDoesNothing)
{
	AddRectangle({ 0, 0, 100, 100 });

	EXPECT_NO_THROW(m_controller.DeleteSelectedShape());
	EXPECT_EQ(m_document.GetShapeCount(), 1u);
}

TEST_F(EditorControllerTest, DeleteDuringDragClearsPreview)
{
	AddRectangle({ 100, 100, 100, 100 });
	m_controller.OnMouseDown({ 150, 150 });
	m_controller.OnMouseMove({ 170, 170 });

	m_controller.DeleteSelectedShape();

	EXPECT_EQ(m_state.GetPreviewFrame(), std::nullopt);
}

TEST_F(EditorControllerTest, MouseEventsAfterDeleteDuringDragAreIgnored)
{
	AddRectangle({ 100, 100, 100, 100 });
	m_controller.OnMouseDown({ 150, 150 });
	m_controller.DeleteSelectedShape();

	EXPECT_NO_THROW(m_controller.OnMouseMove({ 200, 200 }));
	EXPECT_NO_THROW(m_controller.OnMouseUp({ 200, 200 }));
	EXPECT_EQ(m_document.GetShapeCount(), 0u);
}

TEST_F(EditorControllerTest, MoveDuringGestureDoesNotChangeDocument)
{
	const auto id = AddRectangle({ 100, 100, 200, 100 });
	m_document.AddObserver(m_documentObserver);
	EXPECT_CALL(m_documentObserver, DocumentUpdate).Times(0);

	m_controller.OnMouseDown({ 150, 150 });
	m_controller.OnMouseMove({ 160, 160 });
	m_controller.OnMouseMove({ 180, 190 });

	EXPECT_EQ(GetFrame(id), (Frame{ 100, 100, 200, 100 }));
}

TEST_F(EditorControllerTest, MoveDuringGestureUpdatesPreviewFrame)
{
	AddRectangle({ 100, 100, 200, 100 });

	m_controller.OnMouseDown({ 150, 150 });
	m_controller.OnMouseMove({ 180, 190 });

	EXPECT_EQ(m_state.GetPreviewFrame(), (Frame{ 130, 140, 200, 100 }));
}

TEST_F(EditorControllerTest, WholeGestureChangesDocumentOnce)
{
	AddRectangle({ 100, 100, 200, 100 });
	m_document.AddObserver(m_documentObserver);
	EXPECT_CALL(m_documentObserver, DocumentUpdate).Times(1);

	Drag({ 150, 150 }, { 190, 200 });
}

TEST_F(EditorControllerTest, MouseUpAppliesFrameAndClearsPreview)
{
	const auto id = AddRectangle({ 100, 100, 200, 100 });

	Drag({ 150, 150 }, { 190, 200 });

	EXPECT_EQ(GetFrame(id), (Frame{ 140, 150, 200, 100 }));
	EXPECT_EQ(m_state.GetPreviewFrame(), std::nullopt);
}

TEST_F(EditorControllerTest, ResizeDuringGestureChangesOnlyPreview)
{
	const auto id = AddSelectedRectangle({ 100, 100, 200, 100 });

	m_controller.OnMouseDown({ 300, 200 });
	m_controller.OnMouseMove({ 350, 250 });

	EXPECT_EQ(GetFrame(id), (Frame{ 100, 100, 200, 100 }));
	EXPECT_EQ(m_state.GetPreviewFrame(), (Frame{ 100, 100, 250, 150 }));
}

TEST_F(EditorControllerTest, HandlesFollowPreviewFrameDuringResize)
{
	AddSelectedRectangle({ 100, 100, 200, 100 });

	m_controller.OnMouseDown({ 300, 200 });
	m_controller.OnMouseMove({ 350, 250 });

	EXPECT_EQ(m_controller.HitTest({ 350, 250 }).m_area, HitArea::ResizeHandle);
}

TEST_F(EditorControllerTest, ClickWithoutMovingDoesNotChangeDocument)
{
	AddRectangle({ 100, 100, 200, 100 });
	m_document.AddObserver(m_documentObserver);
	EXPECT_CALL(m_documentObserver, DocumentUpdate).Times(0);

	Click({ 150, 150 });
}

TEST_F(EditorControllerTest, NewGestureWithoutMouseUpDropsOldPreview)
{
	AddRectangle({ 100, 100, 100, 100 });
	AddRectangle({ 500, 300, 100, 100 });
	m_controller.OnMouseDown({ 150, 150 });
	m_controller.OnMouseMove({ 200, 200 });

	m_controller.OnMouseDown({ 550, 350 });

	EXPECT_EQ(m_state.GetPreviewFrame(), std::nullopt);
}

TEST_F(EditorControllerTest, SaveRemembersFilePath)
{
	AddRectangle({ 10, 20, 100, 50 });

	m_controller.SaveDocument(m_path);

	EXPECT_EQ(m_state.GetFilePath(), m_path);
}

TEST_F(EditorControllerTest, NewDocumentClearsShapesAndFilePath)
{
	AddRectangle({ 10, 20, 100, 50 });
	m_controller.SaveDocument(m_path);

	m_controller.NewDocument();

	EXPECT_EQ(m_document.GetShapeCount(), 0u);
	EXPECT_EQ(m_state.GetFilePath(), std::nullopt);
}

TEST_F(EditorControllerTest, OpenRestoresSavedShapes)
{
	AddRectangle({ 10, 20, 100, 50 });
	m_controller.AddShape(ShapeType::Triangle);
	m_controller.SaveDocument(m_path);
	m_controller.NewDocument();

	m_controller.OpenDocument(m_path);

	ASSERT_EQ(m_document.GetShapeCount(), 2u);
	EXPECT_EQ(m_document.GetShape(0).GetType(), ShapeType::Rectangle);
	EXPECT_EQ(m_document.GetShape(0).GetFrame(), (Frame{ 10, 20, 100, 50 }));
	EXPECT_EQ(m_document.GetShape(1).GetType(), ShapeType::Triangle);
}

TEST_F(EditorControllerTest, OpenClearsSelectionAndRemembersFilePath)
{
	WriteFile("rectangle 10 20 100 50 ff0000\n");
	AddSelectedRectangle({ 0, 0, 100, 100 });

	m_controller.OpenDocument(m_path);

	EXPECT_EQ(m_state.GetSelectedShapeId(), std::nullopt);
	EXPECT_EQ(m_state.GetFilePath(), m_path);
}

TEST_F(EditorControllerTest, FailedOpenOfInvalidFileKeepsCurrentDocument)
{
	WriteFile("rectangle 0 0 50 50 ff0000\nhexagon 0 0 50 50 ff0000\n");
	const auto id = AddSelectedRectangle({ 10, 20, 100, 50 });

	EXPECT_THROW(m_controller.OpenDocument(m_path), std::runtime_error);

	ASSERT_EQ(m_document.GetShapeCount(), 1u);
	EXPECT_EQ(GetFrame(id), (Frame{ 10, 20, 100, 50 }));
	EXPECT_EQ(m_state.GetSelectedShapeId(), id);
	EXPECT_EQ(m_state.GetFilePath(), std::nullopt);
}

TEST_F(EditorControllerTest, FailedOpenOfMissingFileKeepsCurrentDocument)
{
	AddRectangle({ 10, 20, 100, 50 });

	EXPECT_THROW(m_controller.OpenDocument(m_path.string() + ".missing"), std::runtime_error);

	EXPECT_EQ(m_document.GetShapeCount(), 1u);
	EXPECT_EQ(m_state.GetFilePath(), std::nullopt);
}

TEST_F(EditorControllerTest, OpenRejectsShapesOutsideCanvas)
{
	WriteFile("rectangle 950 0 100 50 ff0000\n");
	AddRectangle({ 10, 20, 100, 50 });

	EXPECT_THROW(m_controller.OpenDocument(m_path), std::runtime_error);
	EXPECT_EQ(m_document.GetShapeCount(), 1u);
}

TEST_F(EditorControllerTest, OpenRejectsShapesSmallerThanMinimumSize)
{
	WriteFile("rectangle 0 0 5 50 ff0000\n");
	AddRectangle({ 10, 20, 100, 50 });

	EXPECT_THROW(m_controller.OpenDocument(m_path), std::runtime_error);
	EXPECT_EQ(m_document.GetShapeCount(), 1u);
}

TEST_F(EditorControllerTest, DragWithShapeToolCreatesShapeOfThatType)
{
	m_controller.SelectShapeTool(ShapeType::Ellipse);

	Drag({ 100, 100 }, { 300, 250 });

	ASSERT_EQ(m_document.GetShapeCount(), 1u);
	EXPECT_EQ(m_document.GetShape(0).GetType(), ShapeType::Ellipse);
	EXPECT_EQ(m_document.GetShape(0).GetFrame(), (Frame{ 100, 100, 200, 150 }));
}

TEST_F(EditorControllerTest, CreatedShapeIsSelectedAndToolIsReset)
{
	m_controller.SelectShapeTool(ShapeType::Rectangle);

	Drag({ 100, 100 }, { 300, 250 });

	EXPECT_EQ(m_state.GetSelectedShapeId(), m_document.GetShapeId(0));
	EXPECT_EQ(m_state.GetShapeTool(), std::nullopt);
}

TEST_F(EditorControllerTest, CreateOverExistingShapeDoesNotMoveIt)
{
	const auto id = AddRectangle({ 100, 100, 200, 100 });
	m_controller.SelectShapeTool(ShapeType::Triangle);

	Drag({ 150, 150 }, { 250, 180 });

	EXPECT_EQ(GetFrame(id), (Frame{ 100, 100, 200, 100 }));
	ASSERT_EQ(m_document.GetShapeCount(), 2u);
	EXPECT_EQ(m_document.GetShape(1).GetType(), ShapeType::Triangle);
}

TEST_F(EditorControllerTest, CreateDuringGestureChangesOnlyPreview)
{
	m_document.AddObserver(m_documentObserver);
	EXPECT_CALL(m_documentObserver, DocumentUpdate).Times(0);
	m_controller.SelectShapeTool(ShapeType::Rectangle);

	m_controller.OnMouseDown({ 100, 100 });
	m_controller.OnMouseMove({ 200, 150 });

	ASSERT_NE(m_state.GetCreatingShape(), nullptr);
	EXPECT_EQ(m_state.GetCreatingShape()->GetFrame(), (Frame{ 100, 100, 100, 50 }));
	EXPECT_EQ(m_document.GetShapeCount(), 0u);
}

TEST_F(EditorControllerTest, CreatePreviewFollowsCursor)
{
	m_controller.SelectShapeTool(ShapeType::Rectangle);

	m_controller.OnMouseDown({ 100, 100 });
	m_controller.OnMouseMove({ 200, 150 });
	m_controller.OnMouseMove({ 50, 300 });

	EXPECT_EQ(m_state.GetCreatingShape()->GetFrame(), (Frame{ 50, 100, 50, 200 }));
}

TEST_F(EditorControllerTest, WholeCreateGestureChangesDocumentOnceAndClearsPreview)
{
	m_document.AddObserver(m_documentObserver);
	EXPECT_CALL(m_documentObserver, DocumentUpdate).Times(1);
	m_controller.SelectShapeTool(ShapeType::Rectangle);

	Drag({ 100, 100 }, { 300, 250 });

	EXPECT_EQ(m_state.GetCreatingShape(), nullptr);
}

TEST_F(EditorControllerTest, CreateIsLimitedByCanvas)
{
	m_controller.SelectShapeTool(ShapeType::Rectangle);

	Drag({ 900, 500 }, { 5000, 5000 });

	EXPECT_EQ(m_document.GetShape(0).GetFrame(), (Frame{ 900, 500, 100, 100 }));
}

TEST_F(EditorControllerTest, CreateAtCanvasCornerKeepsMinimumSizeInsideCanvas)
{
	m_controller.SelectShapeTool(ShapeType::Rectangle);

	Drag({ 999, 599 }, { 1200, 700 });

	EXPECT_EQ(m_document.GetShape(0).GetFrame(),
		(Frame{ 1000 - ShapeDefaults::MIN_SIZE, 600 - ShapeDefaults::MIN_SIZE, ShapeDefaults::MIN_SIZE, ShapeDefaults::MIN_SIZE }));
}

TEST_F(EditorControllerTest, ClickWithShapeToolDoesNotCreateShape)
{
	m_controller.SelectShapeTool(ShapeType::Rectangle);

	Click({ 100, 100 });

	EXPECT_EQ(m_document.GetShapeCount(), 0u);
	EXPECT_EQ(m_state.GetShapeTool(), ShapeType::Rectangle);
}

TEST_F(EditorControllerTest, CancelDuringCreateDropsPreviewAndIgnoresMouseUp)
{
	m_controller.SelectShapeTool(ShapeType::Rectangle);
	m_controller.OnMouseDown({ 100, 100 });
	m_controller.OnMouseMove({ 200, 200 });

	m_controller.Cancel();
	m_controller.OnMouseMove({ 250, 250 });
	m_controller.OnMouseUp({ 250, 250 });

	EXPECT_EQ(m_state.GetCreatingShape(), nullptr);
	EXPECT_EQ(m_document.GetShapeCount(), 0u);
	EXPECT_EQ(m_state.GetShapeTool(), ShapeType::Rectangle);
}

TEST_F(EditorControllerTest, CancelWithoutGestureResetsShapeTool)
{
	m_controller.SelectShapeTool(ShapeType::Rectangle);

	m_controller.Cancel();

	EXPECT_EQ(m_state.GetShapeTool(), std::nullopt);
}

TEST_F(EditorControllerTest, CancelDuringMoveRestoresShapeFrame)
{
	const auto id = AddRectangle({ 100, 100, 200, 100 });
	m_controller.OnMouseDown({ 150, 150 });
	m_controller.OnMouseMove({ 250, 250 });

	m_controller.Cancel();
	m_controller.OnMouseUp({ 250, 250 });

	EXPECT_EQ(GetFrame(id), (Frame{ 100, 100, 200, 100 }));
	EXPECT_EQ(m_state.GetPreviewFrame(), std::nullopt);
}

TEST_F(EditorControllerTest, CursorIsCrossWithShapeTool)
{
	AddSelectedRectangle({ 100, 100, 200, 100 });

	m_controller.SelectShapeTool(ShapeType::Ellipse);

	EXPECT_EQ(m_controller.GetCursorType({ 100, 100 }), CursorType::Cross);
}
