#pragma once

#include <vector>
#include <memory>

static const size_t default_size = 10;

template<typename T>
class ObjectPool
{
private:
	std::unique_ptr<char[]> pool;	// pool
	std::vector<bool> usage;		// vector of booleans assossiated with pool
	size_t size{};					// size of pool
	T* data{};						// ptr to the head of pool

public:
	ObjectPool(size_t _size = default_size) : size(_size)
	{
		pool = std::make_unique<char[]>(sizeof(T) * size); // std::unique_ptr<char[]>(new char[sizeof(T) * size])
		data = reinterpret_cast<T*>(pool.get());
		usage.resize(size, false);
	}

	template<typename... Args>
	T& alloc(Args&&... args)
	{
		for (size_t i = 0; i < size; ++i)
		{
			if (usage[i] == false)
			{
				usage[i] = true;
				T* ptr = new(data + i) T{ std::forward<Args>(args)... };
				return *ptr;
			}
		}
		throw std::out_of_range("ObjectPool is full!\n");
	}

	void free(T& object)
	{
		size_t i(-1);

		T* object_ptr = &object;
		if (object_ptr >= data)
		{
			i = object_ptr - data;

#ifdef _DEBUG
			std::cout << i;
#endif // _DEBUG
		}

		if (i < size && usage[i] == true)
		{
			usage[i] = false;
			object_ptr->~T();
		}
		else
		{
			throw std::out_of_range("Could not free object!\n");
		}
	}

	size_t num_free()
	{
		size_t num_free(0);
		for (size_t i = 0; i < size; ++i)
		{
			if (usage[i] == false)
			{
				++num_free;
			}
		}
		return num_free;
	}

	~ObjectPool()
	{
		for (size_t i = 0; i < size; ++i)
		{
			if (usage[i] == true)
			{
				T* object_ptr = data + i;
				object_ptr->~T();
			}
		}
	}
};