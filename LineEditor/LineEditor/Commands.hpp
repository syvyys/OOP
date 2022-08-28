#pragma once

#include <string>
#include <memory>
#include <vector>
#include <string>
#include "Document.hpp"

class Document;

class Command
{
public:
	Command() = delete;
	Command(std::shared_ptr<Document> _doc) : doc(_doc) {}
	virtual void exec() = 0;
	virtual void undo() = 0;
	static std::shared_ptr<Command> create(std::shared_ptr<Document> _doc,
		const std::vector<std::string>& params);

protected:
	std::shared_ptr<Document> doc;
};

class CopyCommand : public Command
{
public:
	CopyCommand() = delete;
	CopyCommand(std::shared_ptr<Document> _doc, const std::vector<std::string>& params);
	void exec();
	void undo();

private:
	int idx1{}, idx2{};
	std::string saved;
};

class PasteCommand : public Command
{
public:
	PasteCommand() = delete;
	PasteCommand(std::shared_ptr<Document> _doc, const std::vector<std::string>& params);
	void exec();
	void undo();

private:
	int idx{}, size{};
};

class InsertCommand : public Command
{
public:
	InsertCommand() = delete;
	InsertCommand(std::shared_ptr<Document> _doc, const std::vector<std::string>& params);
	void exec();
	void undo();

private:
	int idx{};
	std::string str{};
};

class EraseCommand : public Command
{
public:
	EraseCommand() = delete;
	EraseCommand(std::shared_ptr<Document> _doc, const std::vector<std::string>& params);
	void exec();
	void undo();

private:
	int idx1{}, idx2{};
	std::string saved;
};

class UndoCommand : public Command
{
public:
	UndoCommand(std::shared_ptr<Document> _doc) : Command(_doc) {};
	virtual void exec() {}
	virtual void undo() {}
};

class RedoCommand : public Command
{
public:
	RedoCommand(std::shared_ptr<Document> _doc) : Command(_doc) {};
	virtual void exec() {}
	virtual void undo() {}
};