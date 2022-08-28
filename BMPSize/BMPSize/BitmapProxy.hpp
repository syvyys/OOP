#pragma once

#include <string>
#include <memory>
#include <iostream>
#include <filesystem>
#include "libbitmap-master/libbitmap.h"

class BitmapProxy
{
	using Size = std::pair<size_t, size_t>;

public:
	BitmapProxy() = default;

	void load(const std::string& _path)
	{
		path = _path;
		ready = false;
		modify_path_if_necessary(); // add .bmp extension, if there's no
	}

	Size get_size()
	{
		if (!ready)
		{
			count_size();
			ready = true;
		}
		return size;
	}

	bool is_ready()
	{
		return ready;
	}

private:
	Size size{ 0, 0 };
	bool ready{ true };
	std::string path{};

	void modify_path_if_necessary()
	{
		size_t off = path.size() - 4; // 4 = ".bmp" length
		size_t pos = path.find(".bmp", off);
		if (pos == -1)
		{
			path += ".bmp";
		}
	}

	void count_size_from_bmp()
	{
		Bitmap bmp;
		bmp.load(path);
		size = { bmp.m_width, bmp.m_height }; // if there's no such file, then h and w are 0 by default
	}

	void count_size()
	{
		size_t pos_ = path.find_last_of('_');
		size_t pos_x = path.find_last_of('x');
		if (pos_ == -1 || pos_x == -1 || pos_x < pos_ || path.size() < 4) // format is wrong
		{
			return count_size_from_bmp();
		}

		long long num1 = 0, num2 = 0;
		size_t idx1 = 0, idx2 = 0;
		size_t size1 = pos_x - pos_ - 1;
		size_t size2 = (path.size() - 4) - pos_x - 1; // pos_bmp == path.size() - 4
		try
		{
			num1 = std::stoll(path.substr(pos_ + 1, size1), &idx1);
			num2 = std::stoll(path.substr(pos_x + 1, size2), &idx2);
		}
		catch (std::exception&)
		{
			return count_size_from_bmp();
		}

		if (idx1 < size2 || idx2 < size2 || num1 < 0 || num2 < 0)
		{
			return count_size_from_bmp();
		}

		size = { num1, num2 };
	}
};
