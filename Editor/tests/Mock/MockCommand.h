//
// Created by Вадим Патрушев on 26.10.2026.
//

#ifndef OOD_EDITOR_MOCKCOMMAND_H
#define OOD_EDITOR_MOCKCOMMAND_H

#include "Command/ICommand.h"
#include <gmock/gmock.h>

class MockCommand : public ICommand
{
public:
	MOCK_METHOD(void, Execute, (), (override));
	MOCK_METHOD(void, Unexecute, (), (override));
};

#endif // OOD_EDITOR_MOCKCOMMAND_H
