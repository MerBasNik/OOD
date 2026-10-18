//
// Created by Вадим Патрушев on 15.10.2026.
//

#include "Model/Serialization/DocumentSerializer.h"
#include "Model/Shapes/Ellipse.h"
#include "Model/Shapes/Rectangle.h"
#include "Model/Shapes/Triangle.h"
#include <gtest/gtest.h>
#include <sstream>

class DocumentSerializerTest : public ::testing::Test
{
public:
	Document document{ 1000, 600 };

	static std::vector<std::unique_ptr<IShape>> LoadFromString(const std::string& text)
	{
		std::istringstream input(text);
		return DocumentSerializer::Load(input);
	}

	std::vector<std::unique_ptr<IShape>> SaveAndLoad() const
	{
		std::stringstream stream;
		DocumentSerializer::Save(document, stream);
		return DocumentSerializer::Load(stream);
	}
};

TEST_F(DocumentSerializerTest, SaveWritesShapesInLayerOrder)
{
	document.AddShape(std::make_unique<Rectangle>(Frame{ 10, 20, 30, 40 }, 0xFF0000));
	document.AddShape(std::make_unique<Ellipse>(Frame{ 1.5, 2.25, 100, 50 }, 0x00ff00));

	std::ostringstream output;
	DocumentSerializer::Save(document, output);

	EXPECT_EQ(output.str(),
		"editor-document 1\n"
		"rectangle 10 20 30 40 ff0000\n"
		"ellipse 1.5 2.25 100 50 00ff00\n");
}

TEST_F(DocumentSerializerTest, SaveEmptyDocumentWritesOnlyHeader)
{
	std::ostringstream output;
	DocumentSerializer::Save(document, output);

	EXPECT_EQ(output.str(), "editor-document 1\n");
}

TEST_F(DocumentSerializerTest, RoundTripRestoresTypesInLayerOrder)
{
	document.AddShape(std::make_unique<Triangle>(Frame{ 0, 0, 30, 30 }, 0));
	document.AddShape(std::make_unique<Rectangle>(Frame{ 0, 0, 30, 30 }, 0));
	document.AddShape(std::make_unique<Ellipse>(Frame{ 0, 0, 30, 30 }, 0));

	const auto shapes = SaveAndLoad();

	ASSERT_EQ(shapes.size(), 3u);
	EXPECT_EQ(shapes[0]->GetType(), ShapeType::Triangle);
	EXPECT_EQ(shapes[1]->GetType(), ShapeType::Rectangle);
	EXPECT_EQ(shapes[2]->GetType(), ShapeType::Ellipse);
}

TEST_F(DocumentSerializerTest, RoundTripRestoresFractionalFrameExactly)
{
	const Frame frame{ 0.1, 0.2, 300.3, 200.7 };
	document.AddShape(std::make_unique<Triangle>(frame, 0));

	const auto shapes = SaveAndLoad();

	ASSERT_EQ(shapes.size(), 1u);
	EXPECT_EQ(shapes[0]->GetFrame(), frame);
}

TEST_F(DocumentSerializerTest, RoundTripRestoresColor)
{
	document.AddShape(std::make_unique<Rectangle>(Frame{ 10, 20, 30, 40 }, 0xABCDEF));

	const auto shapes = SaveAndLoad();

	ASSERT_EQ(shapes.size(), 1u);
	EXPECT_EQ(shapes[0]->GetColor(), 0xABCDEFu);
}

TEST_F(DocumentSerializerTest, SkipsEmptyLines)
{
	const auto shapes = LoadFromString("\neditor-document 1\n\n  \ntriangle 1 2 3 4 000000\n\n");

	ASSERT_EQ(shapes.size(), 1u);
	EXPECT_EQ(shapes[0]->GetType(), ShapeType::Triangle);
}

TEST_F(DocumentSerializerTest, LoadFromMissingFileThrows)
{
	EXPECT_THROW(DocumentSerializer::LoadFromFile("/nonexistent/dir/file.shapes"), std::runtime_error);
}

TEST_F(DocumentSerializerTest, SaveToFileReplacesPreviousVersionWithoutTempFile)
{
	const auto path = std::filesystem::temp_directory_path() / "document_serializer_test.shapes";
	DocumentSerializer::SaveToFile(document, path);
	document.AddShape(std::make_unique<Rectangle>(Frame{ 10, 20, 30, 40 }, 0));

	DocumentSerializer::SaveToFile(document, path);

	EXPECT_EQ(DocumentSerializer::LoadFromFile(path).size(), 1u);
	EXPECT_FALSE(std::filesystem::exists(path.string() + ".tmp"));
	std::filesystem::remove(path);
}

TEST_F(DocumentSerializerTest, SaveToMissingDirectoryThrows)
{
	EXPECT_THROW(DocumentSerializer::SaveToFile(document, "/nonexistent/dir/file.shapes"), std::runtime_error);
}

TEST_F(DocumentSerializerTest, FailedSaveKeepsPreviousFileAndRemovesTempFile)
{
	const auto path = std::filesystem::temp_directory_path() / "document_serializer_dir.shapes";
	std::filesystem::create_directory(path);

	EXPECT_THROW(DocumentSerializer::SaveToFile(document, path), std::runtime_error);

	EXPECT_TRUE(std::filesystem::is_directory(path));
	EXPECT_FALSE(std::filesystem::exists(path.string() + ".tmp"));
	std::filesystem::remove(path);
}

struct InvalidDocumentCase
{
	std::string m_name;
	std::string m_text;
	std::string m_expectedMessagePart;
};

class DocumentSerializerInvalidContentTest : public ::testing::TestWithParam<InvalidDocumentCase>
{
};

TEST_P(DocumentSerializerInvalidContentTest, ReportsErrorWithDescription)
{
	const auto& [name, text, expectedMessagePart] = GetParam();
	try
	{
		DocumentSerializerTest::LoadFromString(text);
		FAIL() << "Ожидалась ошибка формата для: " << text;
	}
	catch (const std::runtime_error& e)
	{
		EXPECT_NE(std::string(e.what()).find(expectedMessagePart), std::string::npos) << e.what();
	}
}

INSTANTIATE_TEST_SUITE_P(DocumentSerializerTest, DocumentSerializerInvalidContentTest,
	::testing::Values(
		InvalidDocumentCase{ "EmptyFile", "", "пуст" },
		InvalidDocumentCase{ "WrongHeader", "hello world\n", "не является документом" },
		InvalidDocumentCase{ "UnsupportedVersion", "editor-document 2\n", "версия" },
		InvalidDocumentCase{ "UnknownShapeType", "editor-document 1\nhexagon 0 0 10 10 000000\n", "неподдерживаемый тип фигуры" },
		InvalidDocumentCase{ "MissingHeight", "editor-document 1\nrectangle 0 0 10\n", "отсутствует параметр height" },
		InvalidDocumentCase{ "MissingColor", "editor-document 1\nrectangle 0 0 10 10\n", "отсутствует параметр color" },
		InvalidDocumentCase{ "NotANumber", "editor-document 1\nrectangle 0 abc 10 10 000000\n", "top" },
		InvalidDocumentCase{ "NumberWithSuffix", "editor-document 1\nrectangle 0 1x 10 10 000000\n", "top" },
		InvalidDocumentCase{ "NanNumber", "editor-document 1\nrectangle 0 nan 10 10 000000\n", "top" },
		InvalidDocumentCase{ "NegativeSize", "editor-document 1\nrectangle 0 0 -10 10 000000\n", "положительными" },
		InvalidDocumentCase{ "InvalidColor", "editor-document 1\nrectangle 0 0 10 10 red\n", "цвет" },
		InvalidDocumentCase{ "ExtraData", "editor-document 1\nrectangle 0 0 10 10 000000 extra\n", "лишние данные" },
		InvalidDocumentCase{ "CommentBeforeHeader", "# comment\neditor-document 1\n", "не является документом" },
		InvalidDocumentCase{ "CommentBetweenShapes", "editor-document 1\n# comment\n", "Строка 2: неподдерживаемый тип фигуры" },
		InvalidDocumentCase{ "ErrorContainsLineNumber", "editor-document 1\nellipse 0 0 10 10 000000\nbad\n", "Строка 3" }),
	[](const ::testing::TestParamInfo<InvalidDocumentCase>& info) { return info.param.m_name; });
