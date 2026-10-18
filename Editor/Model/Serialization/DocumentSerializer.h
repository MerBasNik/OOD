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
	static bool IsSupportedTypeName(const std::string& name);
	static std::string LinePrefix(size_t lineNumber);
	static double ParseNumber(const std::string& token, const std::string& name, size_t lineNumber);
	static Color ParseColor(const std::string& token, size_t lineNumber);
	static std::unique_ptr<IShape> ParseShape(const std::string& line, size_t lineNumber);
	static bool IsEmptyLine(const std::string& line);
};

#endif // OOD_EDITOR_DOCUMENTSERIALIZER_H
