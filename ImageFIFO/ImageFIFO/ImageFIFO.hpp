#pragma once

#include <mutex>
#include <deque>
#include <memory>
#include <vector>

class ImageFIFO
{
public:
	ImageFIFO(size_t block_size, size_t max_blocks);

	void* get_free();
	void* get_ready();

	void add_free(void* ptr);
	void add_ready(void* ptr);

	size_t num_free();
	size_t num_busy();
	size_t num_ready();

private:
	std::vector<std::unique_ptr<char[]>> data;
	std::deque<void*> ready;
	std::vector<bool> free;

	size_t size{};
	size_t max{};

	std::mutex mutex_ready{};
	std::mutex mutex_free{};

	bool is_ready(void* ptr);
};