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
	static std::optional<std::filesystem::path> ShowOpenFileDialog();
	static std::optional<std::filesystem::path> ShowSaveFileDialog(const std::string& defaultName);
	static void ShowError(const std::string& message);

private:
	static std::string QuoteShellArgument(const std::string& text);
	static std::string RunAppleScript(const std::string& script, const std::string& argument = "");
	static std::optional<std::filesystem::path> ToPath(const std::string& output);
};

#endif // OOD_EDITOR_FILEWINDOWS_H
