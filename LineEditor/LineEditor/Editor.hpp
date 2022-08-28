#pragma once

#include <vector>
#include <memory>
#include "Document.hpp"
#include "Commands.hpp"

class Editor
{
public:
	Editor() = delete;
	Editor(std::shared_ptr<Document> _doc) : doc(_doc) {}
	void exec(std::shared_ptr<Command> cmd);
	void exec(std::vector<std::shared_ptr<Command>>& cmds);
	void undo();
	void redo();

private:
	std::shared_ptr<Document> doc;
	std::vector<std::shared_ptr<Command>> executed_commands;
	std::vector<std::shared_ptr<Command>> canceled_commands;
};