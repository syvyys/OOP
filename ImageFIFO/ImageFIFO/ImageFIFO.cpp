#include "ImageFIFO.hpp"

ImageFIFO::ImageFIFO(size_t block_size, size_t max_blocks)
{
	for (size_t i = 0; i < max_blocks; ++i)
	{
		data.push_back(std::make_unique<char[]>(block_size));
	}

	free.resize(max_blocks, true);
	size = block_size;
	max = max_blocks;
}

void* ImageFIFO::get_free()
{
	std::lock_guard<std::mutex> guard(mutex_free);
	for (size_t i = 0; i < max; ++i)
	{
		if (free[i])
		{
			free[i] = false;
			return static_cast<void*>(data[i].get());
		}
	}
	return nullptr;
}

void* ImageFIFO::get_ready()
{
	std::lock_guard<std::mutex> guard(mutex_ready);
	if (!ready.empty())
	{
		void* ptr = ready.front();
		ready.pop_front();
		return ptr;
	}
	return nullptr;
}

void ImageFIFO::add_free(void* ptr)
{
	std::lock_guard<std::mutex> guard(mutex_free);
	for (size_t i = 0; i < max; ++i)
	{
		if (ptr == static_cast<void*>(data[i].get()) && !free[i])
		{
			free[i] = true;
			return;
		}
	}
}

void ImageFIFO::add_ready(void* ptr)
{
	std::lock_guard<std::mutex> guard(mutex_ready);
	for (size_t i = 0; i < max; ++i)
	{
		if (ptr == static_cast<void*>(data[i].get()) && !is_ready(ptr))
		{
			ready.push_back(static_cast<void*>(data[i].get()));
			return;
		}
	}
}

size_t ImageFIFO::num_free()
{
	size_t num = 0;
	for (size_t i = 0; i < max; ++i)
	{
		if (free[i])
		{
			++num;
		}
	}
	return num;
}

size_t ImageFIFO::num_busy()
{
	return max - num_free();
}

size_t ImageFIFO::num_ready()
{
	size_t num = 0;
	for (auto it : ready)
	{
		++num;
	}
	return num;
}

bool ImageFIFO::is_ready(void* ptr)
{
	for (auto it : ready)
	{
		if (ptr == it)
		{
			return true;
		}
	}
	return false;
}