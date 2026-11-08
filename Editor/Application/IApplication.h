//
// Created by Вадим Патрушев on 02.11.2026.
//

#ifndef OOD_IAPPLICATION_H
#define OOD_IAPPLICATION_H

#include "AppDocument.h"
#include <filesystem>
#include <memory>

class IApplication
{
public:
	virtual ~IApplication() = default;
	virtual void NewApplication() = 0;
	virtual void OpenApplication(const std::filesystem::path& path) = 0;
	virtual void NewWindow(std::shared_ptr<AppDocument> appDocument) = 0;
};

#endif // OOD_IAPPLICATION_H
