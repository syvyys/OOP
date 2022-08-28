#include <string>
#include <sstream>
#include "Parser.hpp"

Parser::Parser(std::shared_ptr<Document> _doc, std::istream& _is) : doc(_doc), is(_is) {};

void Parser::parse(std::ostream& os)
{
	std::string str;
	while (std::getline(is, str))
	{
		std::vector<std::string> params;
		std::stringstream ss(str);
		std::string temp;
		while (ss >> temp)
		{
			params.push_back(temp);
		}

		try
		{
			commands.push_back(Command::create(doc, params));
		}
		catch (std::invalid_argument&)
		{
			os << "Error in line: " << str << std::endl;
		}
	}
}

std::vector<std::shared_ptr<Command>>& Parser::get_commands()
{
	return commands;
}