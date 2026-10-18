//
// Created by Вадим Патрушев on 15.10.2026.
//

#include "DocumentSerializer.h"
#include "Model/Shapes/Ellipse.h"
#include "Model/Shapes/Rectangle.h"
#include "Model/Shapes/Triangle.h"
#include <cmath>
#include <fstream>
#include <iomanip>
#include <sstream>

const std::string HEADER = "editor-document";
const int VERSION = 1;
const std::string RECTANGLE_NAME = "rectangle";
const std::string ELLIPSE_NAME = "ellipse";
const std::string TRIANGLE_NAME = "triangle";

std::string DocumentSerializer::GetTypeName(const ShapeType type)
{
	switch (type)
	{
	case ShapeType::Rectangle:
		return RECTANGLE_NAME;
	case ShapeType::Ellipse:
		return ELLIPSE_NAME;
	case ShapeType::Triangle:
		return TRIANGLE_NAME;
	}
	throw std::logic_error("Неизвестный тип фигуры");
}

bool DocumentSerializer::IsSupportedTypeName(const std::string& name)
{
	return name == RECTANGLE_NAME || name == ELLIPSE_NAME || name == TRIANGLE_NAME;
}

std::string DocumentSerializer::LinePrefix(const size_t lineNumber)
{
	return "Строка " + std::to_string(lineNumber) + ": ";
}

double DocumentSerializer::ParseNumber(const std::string& token, const std::string& name, const size_t lineNumber)
{
	size_t parsed = 0;
	double value = 0;
	try
	{
		value = std::stod(token, &parsed);
	}
	catch (const std::exception&)
	{
		parsed = 0;
	}
	if (parsed != token.size() || !std::isfinite(value))
	{
		throw std::runtime_error(LinePrefix(lineNumber)
			+ "недопустимое значение параметра " + name + ": \"" + token + "\"");
	}
	return value;
}

Color DocumentSerializer::ParseColor(const std::string& token, const size_t lineNumber)
{
	const bool isHex = token.size() == 6
		&& token.find_first_not_of("0123456789abcdefABCDEF") == std::string::npos;
	if (!isHex)
	{
		throw std::runtime_error(LinePrefix(lineNumber) + "недопустимый цвет: \"" + token + "\"");
	}
	return static_cast<Color>(std::stoul(token, nullptr, 16));
}

std::unique_ptr<IShape> DocumentSerializer::ParseShape(const std::string& line, const size_t lineNumber)
{
	std::istringstream stream(line);
	std::string typeName;
	stream >> typeName;

	if (!IsSupportedTypeName(typeName))
	{
		throw std::runtime_error(LinePrefix(lineNumber) + "неподдерживаемый тип фигуры \"" + typeName + "\"");
	}

	const char* names[] = { "left", "top", "width", "height", "color" };
	std::string tokens[std::size(names)];
	for (size_t i = 0; i < std::size(names); ++i)
	{
		if (!(stream >> tokens[i]))
		{
			throw std::runtime_error(LinePrefix(lineNumber) + "отсутствует параметр " + names[i]);
		}
	}
	std::string extra;
	if (stream >> extra)
	{
		throw std::runtime_error(LinePrefix(lineNumber) + "лишние данные \"" + extra + "\"");
	}

	const Frame frame{
		ParseNumber(tokens[0], names[0], lineNumber),
		ParseNumber(tokens[1], names[1], lineNumber),
		ParseNumber(tokens[2], names[2], lineNumber),
		ParseNumber(tokens[3], names[3], lineNumber),
	};
	if (frame.m_width <= 0 || frame.m_height <= 0)
	{
		throw std::runtime_error(LinePrefix(lineNumber) + "размеры фигуры должны быть положительными");
	}
	const Color color = ParseColor(tokens[4], lineNumber);

	if (typeName == RECTANGLE_NAME)
	{
		return std::make_unique<Rectangle>(frame, color);
	}
	if (typeName == ELLIPSE_NAME)
	{
		return std::make_unique<Ellipse>(frame, color);
	}
	return std::make_unique<Triangle>(frame, color);
}

bool DocumentSerializer::IsEmptyLine(const std::string& line)
{
	return line.find_first_not_of(" \t\r") == std::string::npos;
}

void DocumentSerializer::Save(const Document& document, std::ostream& output)
{
	output << HEADER << " " << VERSION << "\n";
	for (size_t i = 0; i < document.GetShapeCount(); ++i)
	{
		const IShape& shape = document.GetShape(i);
		const Frame frame = shape.GetFrame();
		output << GetTypeName(shape.GetType()) << " "
			   << std::setprecision(17) << frame.m_left << " " << frame.m_top << " "
			   << frame.m_width << " " << frame.m_height << " "
			   << std::hex << std::setw(6) << std::setfill('0') << shape.GetColor()
			   << std::dec << std::setfill(' ') << "\n";
	}
}

std::vector<std::unique_ptr<IShape>> DocumentSerializer::Load(std::istream& input)
{
	std::string line;
	size_t lineNumber = 0;

	bool hasHeader = false;
	while (std::getline(input, line))
	{
		++lineNumber;
		if (IsEmptyLine(line))
		{
			continue;
		}
		std::istringstream stream(line);
		std::string header;
		int version = 0;
		std::string extra;
		if (!(stream >> header >> version) || header != HEADER || (stream >> extra))
		{
			throw std::runtime_error("Файл не является документом редактора");
		}
		if (version != VERSION)
		{
			throw std::runtime_error("Неподдерживаемая версия формата: " + std::to_string(version));
		}
		hasHeader = true;
		break;
	}
	if (!hasHeader)
	{
		throw std::runtime_error("Файл пуст или не является документом редактора");
	}

	std::vector<std::unique_ptr<IShape>> shapes;
	while (std::getline(input, line))
	{
		++lineNumber;
		if (!IsEmptyLine(line))
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
	// Пишем во временный файл и заменяем им целевой, чтобы при сбое не испортить прежнюю версию
	auto tempPath = path;
	tempPath += ".tmp";
	std::ofstream output(tempPath);
	Save(document, output);
	output.close();

	// Поток в состоянии ошибки, если файл не открылся или запись не удалась
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
