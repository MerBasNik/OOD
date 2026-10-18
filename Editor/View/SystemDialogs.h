//
// Created by Вадим Патрушев on 15.10.2026.
//

#ifndef OOD_EDITOR_SYSTEMDIALOGS_H
#define OOD_EDITOR_SYSTEMDIALOGS_H

#include <filesystem>
#include <optional>
#include <string>

class SystemDialogs
{
public:
	static std::optional<std::filesystem::path> ShowOpenFileDialog();
	static std::optional<std::filesystem::path> ShowSaveFileDialog(const std::string& defaultName);
	static void ShowError(const std::string& message);

private:
	static std::string EscapeAppleScriptString(const std::string& text);
	static std::string QuoteShellArgument(const std::string& text);
	static std::optional<std::string> RunScriptAlert(const std::string& script);
	static std::optional<std::filesystem::path> ToPath(const std::optional<std::string>& output);
};

#endif // OOD_EDITOR_SYSTEMDIALOGS_H
