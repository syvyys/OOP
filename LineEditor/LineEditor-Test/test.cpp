#include "gtest/gtest.h"
#include "../LineEditor/Document.hpp"
#include "../LineEditor/Commands.hpp"
#include "../LineEditor/Parser.hpp"
#include "../LineEditor/Editor.hpp"
#include "../LineEditor/LineEditor.hpp"

#include <string>
#include <ostream>
#include <sstream>

std::string empty;
std::string str1 = "0123456789";
std::string str2 = "A fermion is a particle that follows Fermi–Dirac statistics \
and generally has half odd integer spin: spin 1/2, spin 3/2, etc.";

std::string copy_cmd = "copy 1 2";
std::string paste_cmd = "paste 2";
std::string insert_cmd = "insert abcde 3";
std::string erase_cmd = "erase 1 5";
std::string undo_cmd = "undo";
std::string redo_cmd = "redo";

std::string wrong_cmd1 = "copy 4";
std::string wrong_cmd2 = "paste nothing";
std::string wrong_cmd3 = "do something";

/* --------------- DOCUMENT TESTS --------------- */

TEST(document, from_string)
{
	Document doc(str1);
	EXPECT_EQ(doc.get_line(), str1);
	EXPECT_EQ(doc.get_buffer(), empty);
}

TEST(document, from_const_string)
{
	Document doc("text");
	EXPECT_EQ(doc.get_line(), "text");
	EXPECT_EQ(doc.get_buffer(), empty);
}

TEST(document, from_stream)
{
	std::istringstream iss(str1);
	Document doc(iss);
	EXPECT_EQ(doc.get_line(), str1);
	EXPECT_EQ(doc.get_buffer(), empty);
}

TEST(document, insert)
{
	Document doc("abc");
	doc.insert("d", 2);
	EXPECT_EQ(doc.get_line(), "abdc");
	EXPECT_EQ(doc.get_buffer(), empty);
}

TEST(document, erase)
{
	Document doc("12345");
	doc.erase(2, 4);
	EXPECT_EQ(doc.get_line(), "125");
	EXPECT_EQ(doc.get_buffer(), empty);
}

TEST(document, set_buffer)
{
	Document doc(str2);
	doc.set_buffer(str1);
	EXPECT_EQ(doc.get_line(), str2);
	EXPECT_EQ(doc.get_buffer(), str1);
}

TEST(document, print)
{
	std::ostringstream oss;
	Document doc(str1);
	doc.print(oss);
	EXPECT_EQ(oss.str(), str1);
}

/* --------------- PARSER TESTS --------------- */

TEST(parser, before_parse)
{
	std::shared_ptr<Document> doc = std::make_shared<Document>(str1);
	std::istringstream cmd(copy_cmd);
	Parser parser(doc, cmd);
	std::vector<std::shared_ptr<Command>>& v = parser.get_commands();
	EXPECT_EQ(v.size(), 0);
}

TEST(parser, copy_cmd)
{
	std::shared_ptr<Document> doc = std::make_shared<Document>(str1);
	std::istringstream cmd(copy_cmd);
	Parser parser(doc, cmd);

	std::ostringstream err;
	parser.parse(err);
	EXPECT_EQ(err.str(), empty);

	std::vector<std::shared_ptr<Command>>& v = parser.get_commands();
	EXPECT_EQ(v.size(), 1);
	if (v.size())
	{
		if (CopyCommand* copy = dynamic_cast<CopyCommand*>(v[0].get()))
		{
			EXPECT_TRUE(true);
		}
		else
		{
			EXPECT_FALSE(true);
		}
	}
}

TEST(parser, paste_cmd)
{
	std::shared_ptr<Document> doc = std::make_shared<Document>(str1);
	std::istringstream cmd(paste_cmd);
	Parser parser(doc, cmd);

	std::ostringstream err;
	parser.parse(err);
	EXPECT_EQ(err.str(), empty);

	std::vector<std::shared_ptr<Command>>& v = parser.get_commands();
	EXPECT_EQ(v.size(), 1);
	if (v.size())
	{
		if (PasteCommand* copy = dynamic_cast<PasteCommand*>(v[0].get()))
		{
			EXPECT_TRUE(true);
		}
		else
		{
			EXPECT_FALSE(true);
		}
	}
}

TEST(parser, undo_cmd)
{
	std::shared_ptr<Document> doc = std::make_shared<Document>(str1);
	std::istringstream cmd(undo_cmd);
	Parser parser(doc, cmd);

	std::ostringstream err;
	parser.parse(err);
	EXPECT_EQ(err.str(), empty);

	std::vector<std::shared_ptr<Command>>& v = parser.get_commands();
	EXPECT_EQ(v.size(), 1);
	if (v.size())
	{
		if (UndoCommand* copy = dynamic_cast<UndoCommand*>(v[0].get()))
		{
			EXPECT_TRUE(true);
		}
		else
		{
			EXPECT_FALSE(true);
		}
	}
}

TEST(parser, wrong_cmd)
{
	std::shared_ptr<Document> doc = std::make_shared<Document>(str1);
	std::istringstream cmd(wrong_cmd1);
	Parser parser(doc, cmd);

	std::ostringstream err;
	parser.parse(err);
	EXPECT_EQ(err.str(), "Error in line: copy 4\n");

	std::vector<std::shared_ptr<Command>>& v = parser.get_commands();
	EXPECT_EQ(v.size(), 0);
}

TEST(parser, many_cmd)
{
	std::shared_ptr<Document> doc = std::make_shared<Document>(str1);
	std::string many_cmd = copy_cmd + '\n' + paste_cmd + '\n' + wrong_cmd1 + '\n' + wrong_cmd2 + '\n' + erase_cmd;
	std::istringstream cmd(many_cmd);
	Parser parser(doc, cmd);

	std::ostringstream err;
	parser.parse(err);
	std::string expected = "Error in line: " + wrong_cmd1 + '\n'
		+ "Error in line: " + wrong_cmd2 + '\n';
	EXPECT_EQ(err.str(), expected);

	std::vector<std::shared_ptr<Command>>& v = parser.get_commands();
	EXPECT_EQ(v.size(), 3);
}


/* --------------- EDITOR TESTS --------------- */

TEST(editor, exec_empty_cmd)
{
	std::shared_ptr<Document> doc = std::make_shared<Document>(str1);
	Editor edit(doc);

	std::shared_ptr<Command> cmd(nullptr);
	edit.exec(cmd);

	EXPECT_EQ(doc->get_line(), str1);
	EXPECT_EQ(doc->get_buffer(), empty);
}

TEST(editor, exec_copy_cmd)
{
	std::shared_ptr<Document> doc = std::make_shared<Document>(str1);
	Editor edit(doc);

	std::vector<std::string> copy = { "copy", "0", "10" };
	std::shared_ptr<CopyCommand> cmd = std::make_shared<CopyCommand>(doc, copy);
	edit.exec(cmd);

	EXPECT_EQ(doc->get_line(), str1);
	EXPECT_EQ(doc->get_buffer(), str1);
}

TEST(editor, erase_cmd)
{
	std::shared_ptr<Document> doc = std::make_shared<Document>(str1);
	Editor edit(doc);

	std::vector<std::string> erase = { "copy", "0", "10" };
	std::shared_ptr<EraseCommand> cmd = std::make_shared<EraseCommand>(doc, erase);
	edit.exec(cmd);

	EXPECT_EQ(doc->get_line(), empty);
	EXPECT_EQ(doc->get_buffer(), empty);
}

TEST(editor, undo_cmd)
{
	std::shared_ptr<Document> doc = std::make_shared<Document>(str1);
	Editor edit(doc);

	std::vector<std::string> erase = { "copy", "0", "10" };
	std::shared_ptr<EraseCommand> cmd = std::make_shared<EraseCommand>(doc, erase);

	edit.exec(cmd);
	EXPECT_EQ(doc->get_line(), empty);
	EXPECT_EQ(doc->get_buffer(), empty);

	edit.undo();
	EXPECT_EQ(doc->get_line(), str1);
	EXPECT_EQ(doc->get_buffer(), empty);
}

TEST(editor, redo_cmd)
{
	std::shared_ptr<Document> doc = std::make_shared<Document>(str1);
	Editor edit(doc);

	std::vector<std::string> insert = { "insert", "\"abc\"", "10" };
	std::shared_ptr<InsertCommand> cmd = std::make_shared<InsertCommand>(doc, insert);

	edit.exec(cmd);
	EXPECT_EQ(doc->get_line(), str1 + "abc");
	EXPECT_EQ(doc->get_buffer(), empty);

	edit.undo();
	EXPECT_EQ(doc->get_line(), str1);
	EXPECT_EQ(doc->get_buffer(), empty);

	edit.redo();
	EXPECT_EQ(doc->get_line(), str1 + "abc");
	EXPECT_EQ(doc->get_buffer(), empty);
}

TEST(editor, many_undo)
{
	std::shared_ptr<Document> doc = std::make_shared<Document>(str1);
	Editor edit(doc);

	std::vector<std::string> insert = { "insert", "\"abc\"", "10" };
	std::shared_ptr<InsertCommand> cmd = std::make_shared<InsertCommand>(doc, insert);

	edit.undo();
	edit.exec(cmd);
	edit.undo();
	EXPECT_NO_THROW(edit.undo());
	EXPECT_NO_THROW(edit.undo());
	EXPECT_EQ(doc->get_line(), str1);
	EXPECT_EQ(doc->get_buffer(), empty);
}

TEST(editor, many_redo)
{
	std::shared_ptr<Document> doc = std::make_shared<Document>(str1);
	Editor edit(doc);

	std::vector<std::string> insert = { "insert", "\"abc\"", "10" };
	std::shared_ptr<InsertCommand> cmd = std::make_shared<InsertCommand>(doc, insert);

	edit.exec(cmd);
	edit.redo();
	edit.undo();
	edit.redo();
	EXPECT_NO_THROW(edit.redo());
	EXPECT_NO_THROW(edit.redo());
	EXPECT_EQ(doc->get_line(), str1 + "abc");
	EXPECT_EQ(doc->get_buffer(), empty);
};

TEST(editor, many_cmd)
{
	std::shared_ptr<Document> doc = std::make_shared<Document>(str1);
	std::string many_cmd = copy_cmd + '\n' + paste_cmd + '\n' + undo_cmd + '\n' + erase_cmd + '\n' + insert_cmd;
	std::istringstream cmd(many_cmd);
	Parser parser(doc, cmd);

	std::ostringstream err;
	parser.parse(err);
	std::vector<std::shared_ptr<Command>>& v = parser.get_commands();
	EXPECT_EQ(err.str(), empty);
	EXPECT_EQ(v.size(), 5);

	Editor edit(doc);
	edit.exec(v);
	EXPECT_EQ(doc->get_line(), "056bcd789");
	EXPECT_EQ(doc->get_buffer(), "12");
}

/* --------------- LINE EDITOR TESTS --------------- */

TEST(line_editor, small_text)
{
	// A fermion is a particle that follows Fermi–Dirac statistics \
	// and generally has half odd integer spin: spin 1/2, spin 3/2, etc.

	std::istringstream input(str2);
	std::string many_cmd = "copy 9 19\npaste 28\nund\nerase 2 9\nerase\ninsert \"boson\" 2\nwrong\nerase 40 1000";
	std::istringstream cmd(many_cmd);
	std::ostringstream err;
	LineEditor editor(input, cmd, err);

	editor.edit();

	std::string expected_err = "Error in line: und\nError in line: erase\nError in line: wrong\n";
	EXPECT_EQ(err.str(), expected_err);

	std::ostringstream output;
	editor.print(output);
	std::string expected_out = "A boson is a particle that is a particle";
	EXPECT_EQ(output.str(), expected_out);
}