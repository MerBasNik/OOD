//
// Created by Вадим Патрушев on 08.11.2026.
//

#ifndef OOD_APPDOCUMENT_H
#define OOD_APPDOCUMENT_H

#include "Command/CommandHistory/CommandHistory.h"
#include "Model/Document/Document.h"
#include <filesystem>
#include <memory>

class AppDocument {
public:
	AppDocument(double width, double height);
	static std::shared_ptr<Application> Load(const std::filesystem::path& path, double width, double height);
	void Save(const std::filesystem::path& path);

	Document& GetDocument();
	CommandHistory& GetCommandHistory();
	std::optional<std::filesystem::path> GetFilePath() const;

private:
	Document m_document;
	CommandHistory m_commandHistory;
	std::optional<std::filesystem::path> m_filePath;
};

#endif //OOD_APPDOCUMENT_H
