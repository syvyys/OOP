#pragma once

#include "Editor.hpp"
#include "Parser.hpp"
#include "Document.hpp"

class LineEditor
{
public:
	LineEditor(std::istream& input, std::istream& cmd, std::ostream& _errors);
	void edit();
	void print(std::ostream& output);

private:
	std::shared_ptr<Document> doc;
	Editor editor;
	Parser parser;
	std::ostream& errors;
};