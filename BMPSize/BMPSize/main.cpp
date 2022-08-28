#include <iostream>
#include <string>

void modify_path_if_necessary(std::string& path)
{
	size_t off = path.size() - 4; // 4 = ".bmp" length
	size_t pos = path.find(".bmp", off);
	if (pos == -1)
	{
		path += ".bmp";
	}
}

int main()
{
	std::string path = "a_bc.bmp";
	size_t pos_underline = path.find_last_of('_');
	size_t pos_extension = path.find_last_of(".bmp") - 4;

	std::cout << pos_underline << ' ' << pos_extension << std::endl;
	std::cout << path.size();
	

	return 0;
}