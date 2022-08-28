#include "Commands.hpp"

/* ------------------------------ CONSTRUCTORS ------------------------------ */

std::shared_ptr<Command> Command::create(std::shared_ptr<Document> _doc,
	const std::vector<std::string>& params)
{
	std::string command = params.front();
	if (command == "copy")			// case 1: copy command
	{
		return std::make_shared<CopyCommand>(_doc, params);
	}
	else if (command == "paste")	// case 2 : paste command
	{
		return std::make_shared<PasteCommand>(_doc, params);
	}
	else if (command == "insert")	// case 3 : insert command
	{
		return std::make_shared<InsertCommand>(_doc, params);
	}
	else if (command == "erase")	// case 4 : erase command
	{
		return std::make_shared<EraseCommand>(_doc, params);
	}
	else if (command == "undo")		// case 5 : undo
	{
		return std::make_shared<UndoCommand>(_doc);
	}
	else if (command == "redo") 	// case 6 : redo
	{
		return std::make_shared<RedoCommand>(_doc);
	}
	else							// case 7 : not a command
	{
		throw std::invalid_argument("inalid");
	}
}

/* copy idx1 idx2 */
CopyCommand::CopyCommand(std::shared_ptr<Document> _doc, const std::vector<std::string>& params)
	: Command(_doc)
{
	if (params.size() != 3)
	{
		throw std::invalid_argument("invalid");
	}

	try
	{
		idx1 = std::stoi(params[1]);
		idx2 = std::stoi(params[2]);
	}
	catch (const std::exception&)
	{
		throw std::invalid_argument("invalid");
	}
}

/* paste idx */
PasteCommand::PasteCommand(std::shared_ptr<Document> _doc, const std::vector<std::string>& params)
	: Command(_doc)
{
	if (params.size() != 2)
	{
		throw std::invalid_argument("invalid");
	}

	try
	{
		idx = std::stoi(params[1]);
	}
	catch (const std::exception&)
	{
		throw std::invalid_argument("invalid");
	}
}

/* insert string idx */
InsertCommand::InsertCommand(std::shared_ptr<Document> _doc, const std::vector<std::string>& params)
	: Command(_doc)
{
	size_t num = params.size();
	if (num < 3)
	{
		throw std::invalid_argument("invalid");
	}

	try
	{
		idx = std::stoi(params[num - 1]);
	}
	catch (const std::exception&)
	{
		throw std::invalid_argument("invalid");
	}

	if (num == 3)
	{
		str = params[1].substr(1, params[1].size() - 2);
	}
	else
	{
		str += params[1].substr(1) + ' ';
		for (size_t i = 2; i < num - 2; i++)
		{
			str += params[i] + ' ';
		}
		str += params[num - 2].substr(0, params[num - 2].size() - 2);
	}
}

/* erase idx1 idx2 */
EraseCommand::EraseCommand(std::shared_ptr<Document> _doc, const std::vector<std::string>& params)
	: Command(_doc)
{
	if (params.size() != 3)
	{
		throw std::invalid_argument("invalid");
	}

	try
	{
		idx1 = std::stoi(params[1]);
		idx2 = std::stoi(params[2]);
	}
	catch (const std::exception&)
	{
		throw std::invalid_argument("invalid");
	}
}

/* ------------------------------ EXEC & UNDO ------------------------------ */

void CopyCommand::exec()
{
	saved = doc->get_buffer();
	doc->set_buffer(doc->get_line().substr(idx1, idx2));
}

void CopyCommand::undo()
{
	doc->set_buffer(saved);
}

void PasteCommand::exec()
{
	doc->insert(doc->get_buffer(), idx);
	size = doc->get_buffer().size();
}

void PasteCommand::undo()
{
	doc->erase(idx, idx + size);
}

void InsertCommand::exec()
{
	doc->insert(str, idx);
}

void InsertCommand::undo()
{
	doc->erase(idx, idx + str.size());
}

void EraseCommand::exec()
{
	saved = doc->get_line().substr(idx1, idx2);
	doc->erase(idx1, idx2);
}

void EraseCommand::undo()
{
	doc->insert(saved, idx1);
}