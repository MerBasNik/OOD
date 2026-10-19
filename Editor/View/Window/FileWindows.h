//
// Created by Вадим Патрушев on 15.10.2026.
//

#ifndef OOD_EDITOR_FILEWINDOWS_H
#define OOD_EDITOR_FILEWINDOWS_H

#include <filesystem>
#include <optional>
#include <string>

class FileWindows
{
public:
	static std::optional<std::filesystem::path> ShowOpenFileWindow();
	static std::optional<std::filesystem::path> ShowSaveFileWindow(const std::string& defaultName);
	static void ShowError(const std::string& message);

private:
	static std::string GetQuoteArgument(const std::string& text);
	static std::string RunScript(const std::string& script, const std::string& argument = "");
	static std::optional<std::filesystem::path> ToPath(const std::string& output);
};

#endif // OOD_EDITOR_FILEWINDOWS_H
