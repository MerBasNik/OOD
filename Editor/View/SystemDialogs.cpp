//
// Created by Вадим Патрушев on 15.10.2026.
//

#include "SystemDialogs.h"
#include <array>
#include <cstdio>

std::string SystemDialogs::EscapeAppleScriptString(const std::string& text)
{
	std::string result;
	for (const char ch : text)
	{
		if (ch == '\\' || ch == '"')
		{
			result += '\\';
		}
		result += ch;
	}
	return result;
}

std::string SystemDialogs::QuoteShellArgument(const std::string& text)
{
	std::string result = "'";
	for (const char ch : text)
	{
		if (ch == '\'')
		{
			result += "'\\''";
		}
		else
		{
			result += ch;
		}
	}
	return result + "'";
}

std::optional<std::string> SystemDialogs::RunScriptAlert(const std::string& script)
{
	const std::string command = "osascript -e 'activate' -e " + QuoteShellArgument(script) + " 2>/dev/null";
	FILE* pipe = popen(command.c_str(), "r");
	if (!pipe)
	{
		return std::nullopt;
	}

	std::string output;
	std::array<char, 256> buffer{};
	while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe))
	{
		output += buffer.data();
	}
	if (pclose(pipe) != 0)
	{
		return std::nullopt;
	}
	while (!output.empty() && (output.back() == '\n' || output.back() == '\r'))
	{
		output.pop_back();
	}
	return output;
}

std::optional<std::filesystem::path> SystemDialogs::ToPath(const std::optional<std::string>& output)
{
	if (!output || output->empty())
	{
		return std::nullopt;
	}
	return std::filesystem::path(*output);
}

std::optional<std::filesystem::path> SystemDialogs::ShowOpenFileDialog()
{
	return ToPath(RunScriptAlert("POSIX path of (choose file with prompt \"Открыть документ\")"));
}

std::optional<std::filesystem::path> SystemDialogs::ShowSaveFileDialog(const std::string& defaultName)
{
	return ToPath(RunScriptAlert("POSIX path of (choose file name with prompt \"Сохранить документ\" default name \""
		+ EscapeAppleScriptString(defaultName) + "\")"));
}

void SystemDialogs::ShowError(const std::string& message)
{
	RunScriptAlert("display alert \"Ошибка\" message \"" + EscapeAppleScriptString(message)
		+ "\" as critical buttons {\"OK\"} default button \"OK\"");
}
