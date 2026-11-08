//
// Created by Вадим Патрушев on 08.11.2026.
//

#include "AppDocument.h"
#include "Controller/ShapeDefaults.h"
#include "Model/Serialization/DocumentSerializer.h"
#include <stdexcept>

AppDocument::AppDocument(const double width, const double height, std::string name)
	: m_document(width, height)
	, m_name(std::move(name))
{
}

std::shared_ptr<AppDocument> AppDocument::Load(const std::filesystem::path& path, const double width, const double height, std::string name)
{
	auto shapes = DocumentSerializer::LoadFromFile(path);
	auto appDocument = std::make_shared<AppDocument>(width, height, std::move(name));
	const Frame bounds = appDocument->m_document.GetBounds();
	for (const auto& shape : shapes)
	{
		const Frame frame = shape->GetFrame();
		if (!bounds.IsContainsFrame(frame))
		{
			throw std::runtime_error("Фигура выходит за пределы холста");
		}
		if (frame.m_width < ShapeDefaults::MIN_SIZE || frame.m_height < ShapeDefaults::MIN_SIZE)
		{
			throw std::runtime_error("Размер фигуры меньше минимально допустимого");
		}
		const TimeInterval timeInterval = shape->GetTimeInterval();
		if (timeInterval.m_end > appDocument->m_document.GetDuration())
		{
			throw std::runtime_error("Время фигуры выходит за длительность документа");
		}
		if (timeInterval.GetDuration() < ShapeDefaults::MIN_DURATION)
		{
			throw std::runtime_error("Длительность фигуры меньше минимально допустимой");
		}
	}

	appDocument->m_document.ReplaceShapes(std::move(shapes));
	appDocument->m_filePath = path;
	return appDocument;
}

void AppDocument::Save(const std::filesystem::path& path)
{
	DocumentSerializer::SaveToFile(m_document, path);
	m_filePath = path;
}

Document& AppDocument::GetDocument()
{
	return m_document;
}

CommandHistory& AppDocument::GetCommandHistory()
{
	return m_commandHistory;
}

std::optional<std::filesystem::path> AppDocument::GetFilePath() const
{
	return m_filePath;
}

const std::string& AppDocument::GetName() const
{
	return m_name;
}

int AppDocument::NextWindowNumber()
{
	return ++m_windowCount;
}
