//
// Created by Вадим Патрушев on 15.10.2026.
//

#ifndef OOD_EDITOR_DOCUMENTSERIALIZER_H
#define OOD_EDITOR_DOCUMENTSERIALIZER_H

#include "Model/Document/Document.h"
#include <filesystem>
#include <iosfwd>

class DocumentSerializer
{
public:
	static void Save(const Document& document, std::ostream& output);
	static std::vector<std::unique_ptr<IShape>> Load(std::istream& input);

	static void SaveToFile(const Document& document, const std::filesystem::path& path);
	static std::vector<std::unique_ptr<IShape>> LoadFromFile(const std::filesystem::path& path);
};

#endif // OOD_EDITOR_DOCUMENTSERIALIZER_H
