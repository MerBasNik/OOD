//
// Created by Вадим Патрушев on 02.11.2026.
//

#ifndef OOD_IAPPLICATION_H
#define OOD_IAPPLICATION_H

#include "AppDocument.h"
#include <memory>

class IApplication
{
public:
	virtual ~IApplication() = default;
	virtual void NewApplication();
	virtual void OpenApplication(const std::filesystem::path& path);
	virtual void NewWindow(std::shared_ptr<AppDocument>);
	virtual int Run();
};

#endif //OOD_IAPPLICATION_H
