//
// Created by Вадим Патрушев on 08.11.2026.
//

#ifndef OOD_APPDOCUMENT_H
#define OOD_APPDOCUMENT_H

#include "Command/CommandHistory/CommandHistory.h"
#include "Model/Document/Document.h"
#include <filesystem>
#include <memory>
#include <optional>
#include <string>

class AppDocument
{
public:
	AppDocument(double width, double height, std::string name);

	static std::shared_ptr<AppDocument> Load(const std::filesystem::path& path, double width, double height, std::string name);
	void Save(const std::filesystem::path& path);
	Document& GetDocument();
	CommandHistory& GetCommandHistory();
	std::optional<std::filesystem::path> GetFilePath() const;
	const std::string& GetName() const;
	int NextWindowNumber();

private:
	Document m_document;
	CommandHistory m_commandHistory;
	std::optional<std::filesystem::path> m_filePath;
	std::string m_name;
	int m_windowCount = 0;
};

#endif // OOD_APPDOCUMENT_H
