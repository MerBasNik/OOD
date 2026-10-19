//
// Created by Вадим Патрушев on 15.10.2026.
//

#include "FileWindows.h"
#include <array>
#include <cstdio>

std::string FileWindows::GetQuoteArgument(const std::string& text)
{
	std::string result = "'";
	for (const char ch : text)
	{
		result += ch == '\'' ? std::string("'\\''") : std::string(1, ch);
	}
	return result + "'";
}

std::string FileWindows::RunScript(const std::string& script, const std::string& argument)
{
	const std::string command = "osascript -e 'on run argv' -e 'activate' -e " + GetQuoteArgument(script)
		+ " -e 'end run' " + GetQuoteArgument(argument) + " 2>/dev/null";
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

std::optional<std::filesystem::path> FileWindows::ShowOpenFileWindow()
{
	return ToPath(RunScript("POSIX path of (choose file with prompt \"Открыть документ\")"));
}

std::optional<std::filesystem::path> FileWindows::ShowSaveFileWindow(const std::string& defaultName)
{
	return ToPath(RunScript(
		"POSIX path of (choose file name with prompt \"Сохранить документ\" default name (item 1 of argv))",
		defaultName));
}

void FileWindows::ShowError(const std::string& message)
{
	RunScript("display alert \"Ошибка\" message (item 1 of argv) as critical", message);
}
