#pragma once
#include <cassert>

template <typename Type, typename TDeleter = std::default_delete<Type>>
class SharedPTR
{
	using SharedPTR_t = SharedPTR<Type, TDeleter>;

private:
	Type* ptr;
	int* count;
	TDeleter deleter{};

public:

	/* constructors and destructor */

	SharedPTR() : ptr(nullptr), count(nullptr) {}

	SharedPTR(Type* obj) : ptr(obj), count(nullptr)
	{
		if (ptr != nullptr)
		{
			count = new int(1);
		}
	}

	SharedPTR(SharedPTR_t&& other) noexcept
	{
		ptr = other.ptr;
		count = other.count;
		other.ptr = nullptr;
		other.count = nullptr;
	}

	SharedPTR(const SharedPTR_t& other)
	{
		ptr = other.ptr;
		count = other.count;
		if (ptr != nullptr)
		{
			assert(count != nullptr);
			++(*count);
		}
	}

	~SharedPTR()
	{
		release();
	}

	/* observers */

	Type& operator*() const
	{
		if (ptr != nullptr)
		{
			return *ptr;
		}
		throw std::out_of_range("Dereferencing nullptr.\n");
	}

	Type* operator->() const
	{
		return ptr;
	}

	Type* get() const
	{
		return ptr;
	}

	int reference_count() const
	{
		if (count != nullptr)
		{
			return *count;
		}
		return 0;
	}

	operator bool() const
	{
		return ptr != nullptr;
	}

	const TDeleter* get_deleter() const
	{
		return &deleter;
	}

	/* assignment */

	SharedPTR_t& operator=(SharedPTR_t&& other) noexcept
	{
		swap(other);
		return *this;
	}

	SharedPTR_t& operator=(const SharedPTR_t& other)
	{
		if (this != &other)
		{
			release();
			ptr = other.ptr;
			count = other.count;
			if (ptr != nullptr)
			{
				++(*count);
			}
		}
		return *this;
	}

	SharedPTR_t& operator=(Type* obj)
	{
		reset(obj);
		return *this;
	}

	/* modifiers */

	void release()
	{
		if (count != nullptr)
		{

			if (*count > 0)
			{
				--(*count);
			}

			if (*count <= 0)
			{
				assert(ptr != nullptr);
				deleter(ptr);
				delete count;
			}
		}
		ptr = nullptr;
		count = nullptr;
	}

	void reset(Type* obj = nullptr)
	{
		if (ptr == obj && obj != nullptr)
		{
			throw std::out_of_range("Same pointer is given as the one stored.\n");
		}

		release();
		if (obj != nullptr)
		{
			ptr = obj;
			count = new int(1);
		}
	}

	void swap(SharedPTR_t& other)
	{
		std::swap(ptr, other.ptr);
		std::swap(count, other.count);
	}
};