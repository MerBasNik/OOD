//
// Created by Вадим Патрушев on 15.10.2026.
//

#include "DocumentSerializer.h"
#include "Model/Shapes/Ellipse.h"
#include "Model/Shapes/GroupShape.h"
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
	Color fillColor = 0;
	Color borderColor = 0;
	stream >> frame.m_left >> frame.m_top >> frame.m_width >> frame.m_height >> std::hex >> fillColor >> borderColor;
	if (!stream || fillColor > MAX_COLOR || borderColor > MAX_COLOR)
	{
		throw std::runtime_error("некорректное описание фигуры \"" + line + "\"");
	}
	const TimeInterval timeInterval = ParseTimeInterval(stream, line);

	std::unique_ptr<IShape> shape;
	if (typeName == ShapeSerializer::RECTANGLE_NAME)
	{
		shape = std::make_unique<Rectangle>(frame, fillColor, borderColor);
	}
	else if (typeName == ShapeSerializer::ELLIPSE_NAME)
	{
		shape = std::make_unique<Ellipse>(frame, fillColor, borderColor);
	}
	else
	{
		shape = std::make_unique<Triangle>(frame, fillColor, borderColor);
	}
	shape->SetTimeInterval(timeInterval);
	return shape;
}

TimeInterval DocumentSerializer::ParseTimeInterval(std::istringstream& stream, const std::string& line)
{
	std::vector<std::string> tokens;
	std::string token;
	while (stream >> token)
	{
		tokens.push_back(token);
	}
	if (tokens.empty())
	{
		return {};
	}
	if (tokens.size() != 2)
	{
		throw std::runtime_error("некорректное описание фигуры \"" + line + "\"");
	}

	TimeInterval timeInterval;
	std::istringstream times(tokens[0] + " " + tokens[1]);
	std::string extra;
	times >> std::dec >> timeInterval.m_start >> timeInterval.m_end;
	if (!times || times >> extra || timeInterval.m_start < 0 || timeInterval.m_start >= timeInterval.m_end)
	{
		throw std::runtime_error("некорректное время фигуры \"" + line + "\"");
	}
	return timeInterval;
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
	std::vector<std::vector<std::unique_ptr<IShape>>> levels(1);
	std::string line;
	size_t lineNumber = 0;
	while (std::getline(input, line))
	{
		++lineNumber;
		std::istringstream stream(line);
		std::string keyword;
		if (!(stream >> keyword))
		{
			continue;
		}
		if (keyword == ShapeSerializer::GROUP_NAME || keyword == ShapeSerializer::GROUP_END_NAME)
		{
			std::string extra;
			if (stream >> extra)
			{
				throw std::runtime_error("некорректное описание группы \"" + line + "\"");
			}
		}
		if (keyword == ShapeSerializer::GROUP_NAME)
		{
			levels.emplace_back();
		}
		else if (keyword == ShapeSerializer::GROUP_END_NAME)
		{
			if (levels.size() < 2)
			{
				throw std::runtime_error("лишний конец группы в строке " + std::to_string(lineNumber));
			}
			auto children = std::move(levels.back());
			levels.pop_back();
			if (children.empty())
			{
				throw std::runtime_error("пустая группа в строке " + std::to_string(lineNumber));
			}
			levels.back().push_back(std::make_unique<GroupShape>(std::move(children)));
		}
		else
		{
			levels.back().push_back(ParseShape(line, lineNumber));
		}
	}
	if (input.bad())
	{
		throw std::runtime_error("Ошибка чтения файла");
	}
	if (levels.size() != 1)
	{
		throw std::runtime_error("группа не закрыта");
	}
	return std::move(levels.front());
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
