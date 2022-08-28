#include <memory>
#include <iostream>
#include "LineEditor.hpp"

LineEditor::LineEditor(std::istream& input, std::istream& cmd, std::ostream& _errors) :
	doc(std::make_shared<Document>(input)), editor(doc), parser(doc, cmd), errors(_errors) {}

void LineEditor::edit()
{
	parser.parse(errors);
	editor.exec(parser.get_commands());
}

void LineEditor::print(std::ostream& output)
{
	doc->print(output);
}