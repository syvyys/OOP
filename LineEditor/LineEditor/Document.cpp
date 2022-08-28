#include "Document.hpp"

Document::Document(const std::string& _line) : line(_line) {}

Document::Document(std::istream& is)
{
	std::getline(is, line);
}

void Document::insert(const std::string& str, int idx)
{
	line.insert(idx, str, 0, str.size());
}

void Document::erase(int idx1, int idx2)
{
	line.erase(idx1, idx2 - idx1);
}

void Document::set_buffer(const std::string& str)
{
	buffer = str;
}

const std::string& Document::get_line()
{
	return line;
}

const std::string& Document::get_buffer()
{
	return buffer;
}

void Document::print(std::ostream& os)
{
	os << line;
}