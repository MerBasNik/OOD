//
// Created by Вадим Патрушев on 15.10.2026.
//

#include "FileWindows.h"
#include <array>
#include <cstdio>

std::string FileWindows::QuoteShellArgument(const std::string& text)
{
	std::string result = "'";
	for (const char ch : text)
	{
		result += ch == '\'' ? std::string("'\\''") : std::string(1, ch);
	}
	return result + "'";
}

std::string FileWindows::RunAppleScript(const std::string& script, const std::string& argument)
{
	const std::string command = "osascript -e 'on run argv' -e 'activate' -e " + QuoteShellArgument(script)
		+ " -e 'end run' " + QuoteShellArgument(argument) + " 2>/dev/null";
	FILE* pipe = popen(command.c_str(), "r");
	if (!pipe)
	{
		return {};
	}

	std::string output;
	std::array<char, 256> buffer{};
	while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe))
	{
		output += buffer.data();
	}
	if (pclose(pipe) != 0)
	{
		return {};
	}
	if (!output.empty() && output.back() == '\n')
	{
		output.pop_back();
	}
	return output;
}

std::optional<std::filesystem::path> FileWindows::ToPath(const std::string& output)
{
	if (output.empty())
	{
		return std::nullopt;
	}
	return std::filesystem::path(output);
}

std::optional<std::filesystem::path> FileWindows::ShowOpenFileDialog()
{
	return ToPath(RunAppleScript("POSIX path of (choose file with prompt \"Открыть документ\")"));
}

std::optional<std::filesystem::path> FileWindows::ShowSaveFileDialog(const std::string& defaultName)
{
	return ToPath(RunAppleScript(
		"POSIX path of (choose file name with prompt \"Сохранить документ\" default name (item 1 of argv))",
		defaultName));
}

void FileWindows::ShowError(const std::string& message)
{
	RunAppleScript("display alert \"Ошибка\" message (item 1 of argv) as critical", message);
}
