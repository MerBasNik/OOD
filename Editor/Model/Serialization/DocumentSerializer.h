//
// Created by Вадим Патрушев on 15.10.2026.
//

#ifndef OOD_EDITOR_DOCUMENTSERIALIZER_H
#define OOD_EDITOR_DOCUMENTSERIALIZER_H

#include "Model/Document/Document.h"
#include <filesystem>
#include <iosfwd>
#include <string>

class DocumentSerializer
{
public:
	static void Save(const Document& document, std::ostream& output);
	static std::vector<std::unique_ptr<IShape>> Load(std::istream& input);

	static void SaveToFile(const Document& document, const std::filesystem::path& path);
	static std::vector<std::unique_ptr<IShape>> LoadFromFile(const std::filesystem::path& path);

private:
	static std::string GetTypeName(ShapeType type);
	static std::unique_ptr<IShape> ParseShape(const std::string& line, size_t lineNumber);
};

#endif // OOD_EDITOR_DOCUMENTSERIALIZER_H
