//
// Created by Вадим Патрушев on 15.10.2026.
//

#include "Mock/InvalidDocumentCase.h"
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
		"rectangle 10 20 30 40 ff0000\n"
		"ellipse 1.5 2.25 100 50 00ff00\n");
}

TEST_F(DocumentSerializerTest, SaveEmptyDocumentWritesNothing)
{
	std::ostringstream output;
	DocumentSerializer::Save(document, output);

	EXPECT_EQ(output.str(), "");
}

TEST_F(DocumentSerializerTest, LoadEmptyInputGivesNoShapes)
{
	EXPECT_TRUE(LoadFromString("").empty());
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
	const auto shapes = LoadFromString("\ntriangle 1 2 3 4 000000\n\n");

	ASSERT_EQ(shapes.size(), 1u);
	EXPECT_EQ(shapes[0]->GetType(), ShapeType::Triangle);
}

TEST_F(DocumentSerializerTest, LoadFromMissingFileThrows)
{
	EXPECT_THROW(DocumentSerializer::LoadFromFile("/abc/file.draw"), std::runtime_error);
}

TEST_F(DocumentSerializerTest, SaveToFileReplacesPreviousVersionWithoutTempFile)
{
	const auto path = std::filesystem::temp_directory_path() / "test.draw";
	DocumentSerializer::SaveToFile(document, path);
	document.AddShape(std::make_unique<Rectangle>(Frame{ 10, 20, 30, 40 }, 0));

	DocumentSerializer::SaveToFile(document, path);

	EXPECT_EQ(DocumentSerializer::LoadFromFile(path).size(), 1u);
	EXPECT_FALSE(std::filesystem::exists(path.string() + ".tmp"));
	std::filesystem::remove(path);
}

TEST_F(DocumentSerializerTest, SaveToMissingDirectoryThrows)
{
	EXPECT_THROW(DocumentSerializer::SaveToFile(document, "/abc/file.draw"), std::runtime_error);
}

TEST_F(DocumentSerializerTest, FailedSaveKeepsPreviousFileAndRemovesTempFile)
{
	const auto path = std::filesystem::temp_directory_path() / "test.draw";
	std::filesystem::create_directory(path);

	EXPECT_THROW(DocumentSerializer::SaveToFile(document, path), std::runtime_error);

	EXPECT_TRUE(std::filesystem::is_directory(path));
	EXPECT_FALSE(std::filesystem::exists(path.string() + ".tmp"));
	std::filesystem::remove(path);
}

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
		InvalidDocumentCase{ "NotAShapeLine", "hello world\n", "неизвестный тип фигуры \"hello\"" },
		InvalidDocumentCase{ "UnknownShapeType", "abc 0 0 10 10 000000\n", "неизвестный тип фигуры \"abc\"" },
		InvalidDocumentCase{ "MissingParameter", "rectangle 0 0 10\n", "некорректное описание фигуры" },
		InvalidDocumentCase{ "MissingColor", "rectangle 0 0 10 10\n", "некорректное описание фигуры" },
		InvalidDocumentCase{ "NotANumber", "rectangle 0 abc 10 10 000000\n", "некорректное описание фигуры" },
		InvalidDocumentCase{ "NumberWithSuffix", "rectangle 0 1x 10 10 000000\n", "некорректное описание фигуры" },
		InvalidDocumentCase{ "NanNumber", "rectangle 0 nan 10 10 000000\n", "некорректное описание фигуры" },
		InvalidDocumentCase{ "InvalidColor", "rectangle 0 0 10 10 red\n", "некорректное описание фигуры" },
		InvalidDocumentCase{ "TooLargeColor", "rectangle 0 0 10 10 1000000\n", "некорректное описание фигуры" },
		InvalidDocumentCase{ "ExtraData", "rectangle 0 0 10 10 000000 extra\n", "некорректное описание фигуры" },
		InvalidDocumentCase{ "Comment", "# comment\n", "неизвестный тип фигуры" },
		InvalidDocumentCase{ "ErrorContainsLineNumber", "ellipse 0 0 10 10 000000\nbad\n", "Строка 2" }),
	[](const ::testing::TestParamInfo<InvalidDocumentCase>& info) { return info.param.m_name; });
