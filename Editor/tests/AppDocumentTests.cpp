//
// Created by Вадим Патрушев on 08.11.2026.
//

#include "Application/AppDocument.h"
#include "Controller/EditorController.h"
#include "Mock/ShapeTypeVisitor.h"
#include "Model/Shapes/Rectangle.h"
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>

constexpr double WIDTH = 1000;
constexpr double HEIGHT = 600;

class AppDocumentTest : public ::testing::Test
{
public:
	AppDocument m_appDocument{ WIDTH, HEIGHT, "Документ 1" };
	Document& m_document = m_appDocument.GetDocument();
	CommandHistory& m_history = m_appDocument.GetCommandHistory();
	EditorState m_state;
	EditorController m_controller{ m_document, m_state, m_history };
	std::filesystem::path m_path = std::filesystem::temp_directory_path() / "test.draw";

	void TearDown() override
	{
		std::filesystem::remove(m_path);
	}

	int AddRectangle(const Frame& frame)
	{
		return m_document.AddShape(std::make_unique<Rectangle>(frame, 0, 0));
	}

	void WriteFile(const std::string& content) const
	{
		std::ofstream file(m_path);
		file << content;
	}

	std::shared_ptr<AppDocument> Load() const
	{
		return AppDocument::Load(m_path, WIDTH, HEIGHT, "Документ 2");
	}
};

TEST_F(AppDocumentTest, NewDocumentIsEmptyWithoutFileAndHistory)
{
	EXPECT_EQ(m_document.GetShapeCount(), 0);
	EXPECT_EQ(m_appDocument.GetFilePath(), std::nullopt);
	EXPECT_FALSE(m_history.CanUndo());
	EXPECT_FALSE(m_history.CanRedo());
}

TEST_F(AppDocumentTest, SaveRemembersFilePath)
{
	AddRectangle({ 10, 20, 100, 50 });

	m_appDocument.Save(m_path);

	EXPECT_EQ(m_appDocument.GetFilePath(), m_path);
}

TEST_F(AppDocumentTest, WindowsOfDocumentGetDifferentNumbers)
{
	EXPECT_EQ(m_appDocument.GetName(), "Документ 1");
	EXPECT_EQ(m_appDocument.NextWindowNumber(), 1);
	EXPECT_EQ(m_appDocument.NextWindowNumber(), 2);
}

TEST_F(AppDocumentTest, SaveKeepsHistory)
{
	m_controller.AddShape(ShapeType::Rectangle);
	m_controller.AddShape(ShapeType::Ellipse);
	m_controller.Undo();

	m_appDocument.Save(m_path);

	EXPECT_TRUE(m_history.CanUndo());
	EXPECT_TRUE(m_history.CanRedo());
}

TEST_F(AppDocumentTest, LoadRestoresSavedShapes)
{
	AddRectangle({ 10, 20, 100, 50 });
	m_controller.AddShape(ShapeType::Triangle);
	m_appDocument.Save(m_path);

	const auto loaded = Load();

	const Document& document = loaded->GetDocument();
	ASSERT_EQ(document.GetShapeCount(), 2);
	EXPECT_EQ(GetShapeType(document.GetShape(0)), ShapeType::Rectangle);
	EXPECT_EQ(document.GetShape(0).GetFrame(), (Frame{ 10, 20, 100, 50 }));
	EXPECT_EQ(GetShapeType(document.GetShape(1)), ShapeType::Triangle);
}

TEST_F(AppDocumentTest, LoadedDocumentHasEmptyHistoryAndRemembersFilePath)
{
	WriteFile("rectangle 10 20 100 50 ff0000 000000\n");

	const auto loaded = Load();

	EXPECT_FALSE(loaded->GetCommandHistory().CanUndo());
	EXPECT_FALSE(loaded->GetCommandHistory().CanRedo());
	EXPECT_EQ(loaded->GetFilePath(), m_path);
}

TEST_F(AppDocumentTest, LoadDoesNotChangeOpenedDocument)
{
	m_controller.AddShape(ShapeType::Rectangle);
	m_appDocument.Save(m_path);
	m_controller.AddShape(ShapeType::Ellipse);

	const auto loaded = Load();

	EXPECT_EQ(m_document.GetShapeCount(), 2);
	EXPECT_TRUE(m_history.CanUndo());
	EXPECT_EQ(loaded->GetDocument().GetShapeCount(), 1);
}

TEST_F(AppDocumentTest, LoadOfInvalidFileThrows)
{
	WriteFile("rectangle 0 0 50 50 ff0000 000000\nhexagon 0 0 50 50 ff0000 000000\n");

	EXPECT_THROW(Load(), std::runtime_error);
}

TEST_F(AppDocumentTest, LoadOfMissingFileThrows)
{
	EXPECT_THROW(AppDocument::Load(m_path.string() + ".missing", WIDTH, HEIGHT, "Документ 2"), std::runtime_error);
}

TEST_F(AppDocumentTest, LoadRejectsShapesOutsideCanvas)
{
	WriteFile("rectangle 950 0 100 50 ff0000 000000\n");

	EXPECT_THROW(Load(), std::runtime_error);
}

TEST_F(AppDocumentTest, LoadRejectsShapesSmallerThanMinimumSize)
{
	WriteFile("rectangle 0 0 5 50 ff0000 000000\n");

	EXPECT_THROW(Load(), std::runtime_error);
}

TEST_F(AppDocumentTest, SaveWritesCommittedColorsOnly)
{
	const auto id = AddRectangle({ 0, 0, 100, 100 });
	m_state.SetSelectedShapeIds({ id });
	m_controller.PreviewFillColor(0x112233);
	m_controller.CommitColors();
	m_controller.PreviewBorderColor(0x445566);

	m_appDocument.Save(m_path);
	const auto loaded = Load();

	ASSERT_EQ(loaded->GetDocument().GetShapeCount(), 1);
	EXPECT_EQ(loaded->GetDocument().GetShape(0).GetFillColor(), 0x112233);
	EXPECT_EQ(loaded->GetDocument().GetShape(0).GetBorderColor(), 0);
}

TEST_F(AppDocumentTest, SaveWritesStateAfterUndo)
{
	const auto id = AddRectangle({ 10, 20, 50, 100 });
	m_state.SetSelectedShapeIds({ id });
	m_controller.OnMouseDown({ 30, 40 });
	m_controller.OnMouseMove({ 100, 200 });
	m_controller.OnMouseUp({ 100, 200 });
	m_controller.AddShape(ShapeType::Ellipse);
	m_controller.Undo();
	m_controller.Undo();

	m_appDocument.Save(m_path);
	const auto loaded = Load();

	ASSERT_EQ(loaded->GetDocument().GetShapeCount(), 1);
	EXPECT_EQ(loaded->GetDocument().GetShape(0).GetFrame(), (Frame{ 10, 20, 50, 100 }));
}

TEST_F(AppDocumentTest, DifferentDocumentsHaveIndependentHistories)
{
	AppDocument other{ WIDTH, HEIGHT, "Документ 2" };
	EditorState otherState;
	EditorController otherController{ other.GetDocument(), otherState, other.GetCommandHistory() };
	m_controller.AddShape(ShapeType::Rectangle);
	otherController.AddShape(ShapeType::Ellipse);

	m_controller.Undo();

	EXPECT_EQ(m_document.GetShapeCount(), 0);
	EXPECT_EQ(other.GetDocument().GetShapeCount(), 1);
	EXPECT_TRUE(other.GetCommandHistory().CanUndo());
	EXPECT_FALSE(m_history.CanUndo());
}

TEST_F(AppDocumentTest, LoadRejectsTimeOutsideDuration)
{
	WriteFile("rectangle 0 0 50 50 ff0000 000000 5 20\n");

	EXPECT_THROW(Load(), std::runtime_error);
}

TEST_F(AppDocumentTest, LoadRejectsTooShortTimeInterval)
{
	WriteFile("rectangle 0 0 50 50 ff0000 000000 5 5.01\n");

	EXPECT_THROW(Load(), std::runtime_error);
}

TEST_F(AppDocumentTest, SaveAndLoadKeepTimeInterval)
{
	const auto id = AddRectangle({ 0, 0, 100, 100 });
	m_document.SetShapeTimeInterval(id, { 2, 6 });

	m_appDocument.Save(m_path);
	const auto loaded = Load();

	ASSERT_EQ(loaded->GetDocument().GetShapeCount(), 1);
	EXPECT_EQ(loaded->GetDocument().GetShape(0).GetTimeInterval(), (TimeInterval{ 2, 6 }));
}
