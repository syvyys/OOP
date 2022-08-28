#include "Editor.hpp"

void Editor::exec(std::shared_ptr<Command> cmd)
{
	if (!cmd.get())
	{
		return;
	}

	if (UndoCommand* undo_cmd = dynamic_cast<UndoCommand*>(cmd.get()))
	{
		undo();
	}
	else if (RedoCommand* undo_cmd = dynamic_cast<RedoCommand*>(cmd.get()))
	{
		redo();
	}
	else
	{
		cmd->exec();
		executed_commands.push_back(std::move(cmd)); // doesn't work without std::move
	}
}

void Editor::exec(std::vector<std::shared_ptr<Command>>& cmds)
{
	for (auto& cmd : cmds)
	{
		exec(cmd);
	}
}

void Editor::undo()
{
	if (!executed_commands.empty())
	{
		executed_commands.back()->undo();
		canceled_commands.push_back(std::move(executed_commands.back()));
		executed_commands.pop_back();
	}
}

void Editor::redo()
{
	if (!canceled_commands.empty())
	{
		canceled_commands.back()->exec();
		executed_commands.push_back(std::move(canceled_commands.back()));
		canceled_commands.pop_back();
	}
}