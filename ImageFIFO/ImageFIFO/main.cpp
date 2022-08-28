#include <mutex>
#include <deque>
#include <thread>
#include <iostream>
#include <algorithm>
#include "ImageFIFO.hpp"

#include <chrono>
using namespace std::chrono;

template <typename T>
void multith_writer(ImageFIFO& fifo, const std::vector<T>& v)
{
	size_t i = 0;
	while (i < v.size())
	{
		T* ptr = (T*)fifo.get_free();
		if (ptr)
		{
			*ptr = v[i];
			fifo.add_ready(ptr);
			++i;
		}
		else
		{
			std::this_thread::sleep_for(10ms);
		}
	}
}

template <typename T>
void multith_reader(ImageFIFO& fifo, std::vector<T>& v, size_t max_size)
{
	size_t size = 0;
	size_t not_ready_count = 0;
	while (size < max_size)
	{
		T* ptr = (T*)fifo.get_ready();
		if (ptr)
		{
			v.push_back(*ptr);
			fifo.add_free(ptr);
			++size;
			not_ready_count = 0;
		}
		else
		{
			if (++not_ready_count >= 100)
			{
				break;
			}
			std::this_thread::sleep_for(10ms);
		}
	}
}

int main()
{
	ImageFIFO fifo(sizeof(char), 2);
	std::vector<char> v1(100, 'a');
	std::vector<char> v2;

	std::thread t1(multith_writer<char>, std::ref(fifo), std::ref(v1));
	std::thread t2(multith_reader<char>, std::ref(fifo), std::ref(v2), v1.size());
	t1.join();
	t2.join();

	if (v1.size() == v2.size())
	{
		std::cout << "It worked" << std::endl;
		bool b = true;
		for (size_t i = 0; i < v1.size(); ++i)
		{
			if (v1[i] != v2[i])
			{
				b = false;
				break;
			}
		}
		std::cout << b << std::endl;
	}
	return 0;
}