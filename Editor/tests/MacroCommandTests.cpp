//
// Created by Вадим Патрушев on 01.11.2026.
//

#include "Command/MacroCommand/MacroCommand.h"
#include "Mock/MockCommand.h"
#include <gtest/gtest.h>

using ::testing::InSequence;
using ::testing::StrictMock;
using ::testing::Throw;

class MacroCommandTest : public ::testing::Test
{
public:
	std::vector<StrictMock<MockCommand>*> m_mocks;

	std::vector<std::unique_ptr<ICommand>> MakeCommands(const size_t count)
	{
		std::vector<std::unique_ptr<ICommand>> commands;
		for (size_t i = 0; i < count; ++i)
		{
			auto command = std::make_unique<StrictMock<MockCommand>>();
			m_mocks.push_back(command.get());
			commands.push_back(std::move(command));
		}
		return commands;
	}
};

TEST_F(MacroCommandTest, ExecuteInOrder)
{
	MacroCommand macro(MakeCommands(3));
	InSequence sequence;
	EXPECT_CALL(*m_mocks[0], Execute);
	EXPECT_CALL(*m_mocks[1], Execute);
	EXPECT_CALL(*m_mocks[2], Execute);

	macro.Execute();
}

TEST_F(MacroCommandTest, UnexecuteInReverseOrder)
{
	MacroCommand macro(MakeCommands(3));
	InSequence sequence;
	EXPECT_CALL(*m_mocks[2], Unexecute);
	EXPECT_CALL(*m_mocks[1], Unexecute);
	EXPECT_CALL(*m_mocks[0], Unexecute);

	macro.Unexecute();
}

TEST_F(MacroCommandTest, FailedExecuteRollsBack)
{
	MacroCommand macro(MakeCommands(3));
	InSequence sequence;
	EXPECT_CALL(*m_mocks[0], Execute);
	EXPECT_CALL(*m_mocks[1], Execute).WillOnce(Throw(std::runtime_error("ошибка")));
	EXPECT_CALL(*m_mocks[0], Unexecute);

	EXPECT_THROW(macro.Execute(), std::runtime_error);
}
