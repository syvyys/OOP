#pragma once
#include <string>
#include <ostream>
#include <iostream>

class Document
{
public:
	Document(const std::string& _line);
	Document(std::istream& is);

	void insert(const std::string& str, int idx);
	void erase(int idx1, int idx2);
	void set_buffer(const std::string& str);

	const std::string& get_line();
	const std::string& get_buffer();

	void print(std::ostream& os);

private:
	std::string line;		// document text
	std::string buffer;		// clipboard
};