//
// Created by Вадим Патрушев on 02.11.2026.
//

#ifndef OOD_APPLICATION_H
#define OOD_APPLICATION_H

#include "AppWindow.h"
#include "IApplication.h"
#include <memory>
#include <vector>

class Application : public IApplication
{
public:
	int Run();
	void NewApplication() override;
	void OpenApplication(const std::filesystem::path& path) override;
	void NewWindow(std::shared_ptr<AppDocument> appDocument) override;

private:
	int m_documentCount = 0;
	std::vector<std::unique_ptr<AppWindow>> m_appWindows;
	std::vector<std::unique_ptr<AppWindow>> m_newAppWindows;

	void AddNewWindows();
	std::string NextDocumentName();
};

#endif // OOD_APPLICATION_H
