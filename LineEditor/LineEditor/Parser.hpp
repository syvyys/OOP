#pragma once

#include <vector>
#include <memory>
#include <ostream>
#include "Commands.hpp"
#include "Document.hpp"

class Parser
{
public:
	Parser() = delete;
	Parser(std::shared_ptr<Document> _doc, std::istream& _is);
	void parse(std::ostream& os); // std::ostream - for errors
	std::vector<std::shared_ptr<Command>>& get_commands();

private:
	std::shared_ptr<Document> doc;
	std::istream& is;
	std::vector<std::shared_ptr<Command>> commands;
};