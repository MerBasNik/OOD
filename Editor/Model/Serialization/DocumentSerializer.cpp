//
// Created by Вадим Патрушев on 15.10.2026.
//

#include "DocumentSerializer.h"
#include "Model/Shapes/Ellipse.h"
#include "Model/Shapes/Rectangle.h"
#include "Model/Shapes/Triangle.h"
#include "ShapeSerializer.h"
#include <fstream>
#include <sstream>

const Color MAX_COLOR = 0xffffff;

std::unique_ptr<IShape> DocumentSerializer::ParseShape(const std::string& line, const size_t lineNumber)
{
	std::istringstream stream(line);
	std::string typeName;
	stream >> typeName;
	if (typeName != ShapeSerializer::RECTANGLE_NAME && typeName != ShapeSerializer::ELLIPSE_NAME
		&& typeName != ShapeSerializer::TRIANGLE_NAME)
	{
		throw std::runtime_error("неизвестный тип фигуры \"" + typeName + "\"");
	}

	Frame frame;
	Color color = 0;
	std::string extra;
	stream >> frame.m_left >> frame.m_top >> frame.m_width >> frame.m_height >> std::hex >> color;
	if (!stream || color > MAX_COLOR || stream >> extra)
	{
		throw std::runtime_error("некорректное описание фигуры \"" + line + "\"");
	}

	if (typeName == ShapeSerializer::RECTANGLE_NAME)
	{
		return std::make_unique<Rectangle>(frame, color);
	}
	if (typeName == ShapeSerializer::ELLIPSE_NAME)
	{
		return std::make_unique<Ellipse>(frame, color);
	}
	return std::make_unique<Triangle>(frame, color);
}

void DocumentSerializer::Save(const Document& document, std::ostream& output)
{
	ShapeSerializer serializer(output);
	for (size_t i = 0; i < document.GetShapeCount(); ++i)
	{
		document.GetShape(i).Accept(serializer);
	}
}

std::vector<std::unique_ptr<IShape>> DocumentSerializer::Load(std::istream& input)
{
	std::vector<std::unique_ptr<IShape>> shapes;
	std::string line;
	size_t lineNumber = 0;
	while (std::getline(input, line))
	{
		++lineNumber;
		if (!line.empty())
		{
			shapes.push_back(ParseShape(line, lineNumber));
		}
	}
	if (input.bad())
	{
		throw std::runtime_error("Ошибка чтения файла");
	}
	return shapes;
}

void DocumentSerializer::SaveToFile(const Document& document, const std::filesystem::path& path)
{
	auto tempPath = path;
	tempPath += ".tmp";
	std::ofstream output(tempPath);
	Save(document, output);
	output.close();

	std::error_code error;
	if (output)
	{
		std::filesystem::rename(tempPath, path, error);
	}
	if (!output || error)
	{
		std::filesystem::remove(tempPath, error);
		throw std::runtime_error("Не удалось сохранить файл: " + path.string());
	}
}

std::vector<std::unique_ptr<IShape>> DocumentSerializer::LoadFromFile(const std::filesystem::path& path)
{
	std::ifstream input(path);
	if (!input)
	{
		throw std::runtime_error("Не удалось открыть файл: " + path.string());
	}
	return Load(input);
}
