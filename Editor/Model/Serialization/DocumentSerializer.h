//
// Created by Вадим Патрушев on 15.10.2026.
//

#ifndef OOD_EDITOR_DOCUMENTSERIALIZER_H
#define OOD_EDITOR_DOCUMENTSERIALIZER_H

#include "Model/Document/Document.h"
#include <filesystem>
#include <iosfwd>
#include <sstream>
#include <string>

class DocumentSerializer
{
public:
	static void Save(const Document& document, std::ostream& output);
	static void Load(std::istream& input, Document& document);
	static void SaveToFile(const Document& document, const std::filesystem::path& path);
	static void LoadFromFile(const std::filesystem::path& path, Document& document);

private:
	static std::unique_ptr<IShape> ParseShape(const std::string& line, Timeline& timeline);
	static TimeInterval ParseTimeInterval(std::istringstream& stream, const std::string& line);
};

#endif // OOD_EDITOR_DOCUMENTSERIALIZER_H
