//
// Created by Вадим Патрушев on 26.10.2026.
//

#include "Command/CommandHistory/CommandHistory.h"
#include "Mock/MockCommand.h"
#include <gtest/gtest.h>

using ::testing::StrictMock;
using ::testing::Throw;

class CommandHistoryTest : public ::testing::Test
{
public:
	CommandHistory m_history;

	StrictMock<MockCommand>& Execute()
	{
		return Execute(m_history);
	}

	static StrictMock<MockCommand>& Execute(CommandHistory& history)
	{
		auto command = std::make_unique<StrictMock<MockCommand>>();
		auto& result = *command;
		EXPECT_CALL(result, Execute);
		history.Execute(std::move(command));
		::testing::Mock::VerifyAndClearExpectations(&result);
		return result;
	}
};

TEST_F(CommandHistoryTest, ExecuteRunsCommandAndAllowsUndo)
{
	Execute();

	EXPECT_TRUE(m_history.CanUndo());
	EXPECT_FALSE(m_history.CanRedo());
}

TEST_F(CommandHistoryTest, UndoUnexecutesCommandsInReverseOrder)
{
	auto& first = Execute();
	auto& second = Execute();
	EXPECT_CALL(second, Unexecute);
	EXPECT_CALL(first, Unexecute);

	m_history.Undo();
	m_history.Undo();

	EXPECT_FALSE(m_history.CanUndo());
	EXPECT_TRUE(m_history.CanRedo());
}

TEST_F(CommandHistoryTest, RedoExecutesCommandsInOriginalOrder)
{
	auto& first = Execute();
	auto& second = Execute();
	EXPECT_CALL(second, Unexecute);
	EXPECT_CALL(first, Unexecute);
	m_history.Undo();
	m_history.Undo();
	EXPECT_CALL(first, Execute);
	EXPECT_CALL(second, Execute);

	m_history.Redo();
	m_history.Redo();

	EXPECT_TRUE(m_history.CanUndo());
	EXPECT_FALSE(m_history.CanRedo());
}

TEST_F(CommandHistoryTest, NewCommandAfterUndoDropsRedo)
{
	Execute();
	auto& undone = Execute();
	EXPECT_CALL(undone, Unexecute);
	m_history.Undo();

	Execute();

	EXPECT_FALSE(m_history.CanRedo());
}

TEST_F(CommandHistoryTest, UndoAtBeginningDoesNothing)
{
	EXPECT_NO_THROW(m_history.Undo());
	EXPECT_FALSE(m_history.CanUndo());
}

TEST_F(CommandHistoryTest, RedoAtEndDoesNothing)
{
	Execute();

	EXPECT_NO_THROW(m_history.Redo());
	EXPECT_FALSE(m_history.CanRedo());
}

TEST_F(CommandHistoryTest, FailedCommandIsNotAddedAndKeepsRedo)
{
	auto& undone = Execute();
	EXPECT_CALL(undone, Unexecute);
	m_history.Undo();
	auto failed = std::make_unique<StrictMock<MockCommand>>();
	EXPECT_CALL(*failed, Execute).WillOnce(Throw(std::runtime_error("ошибка")));

	EXPECT_THROW(m_history.Execute(std::move(failed)), std::runtime_error);

	EXPECT_FALSE(m_history.CanUndo());
	EXPECT_TRUE(m_history.CanRedo());
}

TEST_F(CommandHistoryTest, ClearRemovesUndoAndRedo)
{
	Execute();
	auto& undone = Execute();
	EXPECT_CALL(undone, Unexecute);
	m_history.Undo();

	m_history.Clear();

	EXPECT_FALSE(m_history.CanUndo());
	EXPECT_FALSE(m_history.CanRedo());
}

TEST_F(CommandHistoryTest, ExecuteRejectsEmptyCommand)
{
	EXPECT_THROW(m_history.Execute(nullptr), std::invalid_argument);
}

TEST_F(CommandHistoryTest, OldestCommandIsDroppedWhenLimitIsExceeded)
{
	CommandHistory history(2);
	Execute(history);
	auto& second = Execute(history);
	auto& third = Execute(history);
	EXPECT_CALL(third, Unexecute);
	EXPECT_CALL(second, Unexecute);

	history.Undo();
	history.Undo();

	EXPECT_FALSE(history.CanUndo());
	EXPECT_NO_THROW(history.Undo());
}

TEST_F(CommandHistoryTest, DefaultLimitKeepsLastHundredCommands)
{
	std::vector<StrictMock<MockCommand>*> commands;
	for (size_t i = 0; i <= CommandHistory::DEFAULT_MAX_SIZE; ++i)
	{
		commands.push_back(&Execute());
	}
	for (size_t i = 1; i < commands.size(); ++i)
	{
		EXPECT_CALL(*commands[i], Unexecute);
	}

	for (size_t i = 0; i < CommandHistory::DEFAULT_MAX_SIZE; ++i)
	{
		m_history.Undo();
	}

	EXPECT_FALSE(m_history.CanUndo());
}

TEST_F(CommandHistoryTest, LimitDoesNotDropCommandsAfterUndo)
{
	CommandHistory history(2);
	auto& first = Execute(history);
	auto& second = Execute(history);
	EXPECT_CALL(second, Unexecute);
	history.Undo();
	auto& third = Execute(history);
	EXPECT_CALL(third, Unexecute);
	EXPECT_CALL(first, Unexecute);

	history.Undo();
	history.Undo();

	EXPECT_FALSE(history.CanUndo());
}

TEST_F(CommandHistoryTest, ZeroLimitIsRejected)
{
	EXPECT_THROW(CommandHistory(0), std::invalid_argument);
}
