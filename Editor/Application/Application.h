//
// Created by Вадим Патрушев on 02.11.2026.
//

#ifndef OOD_APPLICATION_H
#define OOD_APPLICATION_H

#include "AppWindow.h"
#include "IApplication.h"
#include "Controller/EditorController.h"

const double CANVAS_WIDTH = 1000;
const double CANVAS_HEIGHT = 650;

class Application : public IApplication
{
public:
	Application();
	void NewApplication() override;
	void OpenApplication(const std::filesystem::path& path) override;
	void NewWindow(std::shared_ptr<AppDocument> appDocument) override;
	int Run() override;

private:
	int m_appCount = 0;
	std::vector<std::unique_ptr<AppWindow>> m_appWindows;
};

#endif //OOD_APPLICATION_H
