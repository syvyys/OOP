#include "gtest/gtest.h"
#include "../ImageFIFO/ImageFIFO.hpp"

#include <vector>
#include <thread>
#include <chrono>

using namespace std::chrono;

struct TestStruct
{
	int a, b;
};

/* one thread */

TEST(GetFree, MaxSizeZero) // make_unique(0) выделет память под один элемент
{
	ImageFIFO fifo(0, 10);
	for (int i = 0; i < 10; ++i)
	{
		void* elem{ nullptr };
		EXPECT_NO_THROW(elem = fifo.get_free());
		EXPECT_TRUE(elem != nullptr);
	}
}

TEST(GetFree, NumBlocksZero)
{
	ImageFIFO fifo(sizeof(char), 0);
	for (int i = 0; i < 10; ++i)
	{
		void* elem;
		EXPECT_NO_THROW(elem = fifo.get_free());
		EXPECT_EQ(elem, nullptr);
	}
}

TEST(GetFree, OneElem)
{
	ImageFIFO fifo(sizeof(char), 10);
	EXPECT_EQ(fifo.num_free(), 10);
	EXPECT_EQ(fifo.num_busy(), 0);

	char* elem{ nullptr };
	EXPECT_NO_THROW(elem = reinterpret_cast<char*>(fifo.get_free()));
	EXPECT_EQ(fifo.num_free(), 9);
	EXPECT_EQ(fifo.num_busy(), 1);
	EXPECT_TRUE(elem != nullptr);
}

TEST(GetFree, AllElems)
{
	ImageFIFO fifo(sizeof(char), 10);

	for (int i = 0; i < 10; ++i)
	{
		char* elem{ nullptr };
		EXPECT_NO_THROW(elem = reinterpret_cast<char*>(fifo.get_free()));
		EXPECT_EQ(fifo.num_free(), 10 - i - 1);
		EXPECT_EQ(fifo.num_busy(), i + 1);
		EXPECT_TRUE(elem != nullptr);
	}
}

TEST(GetFree, TooManyElems)
{
	ImageFIFO fifo(sizeof(char), 10);
	for (int i = 0; i < 10; ++i)
	{
		char* elem{ nullptr };
		EXPECT_NO_THROW(elem = reinterpret_cast<char*>(fifo.get_free()));
		EXPECT_EQ(fifo.num_free(), 10 - i - 1);
		EXPECT_EQ(fifo.num_busy(), i + 1);
		EXPECT_TRUE(elem != nullptr);
	}

	for (int i = 0; i < 10; ++i)
	{
		char* elem;
		EXPECT_EQ(fifo.num_free(), 0);
		EXPECT_EQ(fifo.num_busy(), 10);
		EXPECT_NO_THROW(elem = reinterpret_cast<char*>(fifo.get_free()));
		EXPECT_EQ(elem, nullptr);
	}
}

TEST(AddReady, OneElem)
{
	ImageFIFO fifo(sizeof(int), 10);
	int* elem = reinterpret_cast<int*>(fifo.get_free());
	EXPECT_EQ(fifo.num_ready(), 0);
	EXPECT_NO_THROW(*elem = 2);
	fifo.add_ready(elem);
	EXPECT_EQ(fifo.num_ready(), 1);
}

TEST(AddReady, AllElems)
{
	ImageFIFO fifo(sizeof(int), 10);
	std::vector<int*> elems;
	for (int i = 0; i < 10; ++i)
	{
		elems.push_back(reinterpret_cast<int*>(fifo.get_free()));
		EXPECT_EQ(fifo.num_ready(), 0);
	}

	for (int i = 0; i < 10; ++i)
	{
		fifo.add_ready(elems[i]);
		EXPECT_EQ(fifo.num_ready(), i + 1);
	}
}

TEST(AddReady, ManyTimes)
{
	ImageFIFO fifo(sizeof(int), 10);
	int* elem = reinterpret_cast<int*>(fifo.get_free());
	EXPECT_EQ(fifo.num_ready(), 0);

	EXPECT_NO_THROW(fifo.add_ready(elem));
	EXPECT_EQ(fifo.num_ready(), 1);

	EXPECT_NO_THROW(fifo.add_ready(elem));
	EXPECT_EQ(fifo.num_ready(), 1);
}

TEST(AddReady, ElemNotFromQueue)
{
	ImageFIFO fifo(sizeof(int), 10);
	int* elem1 = reinterpret_cast<int*>(fifo.get_free());
	EXPECT_EQ(fifo.num_ready(), 0);
	char elem2;
	EXPECT_NO_THROW(fifo.add_ready(&elem2));
	EXPECT_EQ(fifo.num_ready(), 0);
}

TEST(AddReady, RandomOrder)
{
	ImageFIFO fifo(sizeof(char), 10);
	std::vector<char*> elems;
	for (int i = 0; i < 10; ++i)
	{
		elems.push_back(reinterpret_cast<char*>(fifo.get_free()));
	}

	for (int i = 0; i < 5; ++i)
	{
		fifo.add_ready(elems[2 * i]);
	}
	EXPECT_EQ(fifo.num_ready(), 5);

	for (int i = 0; i < 10; ++i)
	{
		EXPECT_NO_THROW(fifo.add_ready(elems[i]));
	}
	EXPECT_EQ(fifo.num_ready(), 10);
}

TEST(GetReady, NoReady)
{
	ImageFIFO fifo(sizeof(long), 10);
	void* elem;
	EXPECT_NO_THROW(elem = fifo.get_ready());
	EXPECT_EQ(elem, nullptr);
}

TEST(GetReady, BusyButNotReady)
{
	ImageFIFO fifo(sizeof(long), 10);
	void* elem = fifo.get_free();
	EXPECT_TRUE(elem != nullptr);
	EXPECT_NO_THROW(elem = fifo.get_ready());
	EXPECT_EQ(elem, nullptr);
}

TEST(GetReady, OneElem)
{
	ImageFIFO fifo(sizeof(TestStruct), 10);
	TestStruct* elem1 = reinterpret_cast<TestStruct*>(fifo.get_free());
	EXPECT_EQ(fifo.num_ready(), 0);

	elem1->a = 1;
	elem1->b = 2;
	fifo.add_ready(elem1);
	EXPECT_EQ(fifo.num_ready(), 1);

	TestStruct* elem2 = reinterpret_cast<TestStruct*>(fifo.get_ready());
	EXPECT_EQ(fifo.num_ready(), 0);
	EXPECT_EQ(elem1, elem2);
	EXPECT_EQ(elem2->a, 1);
	EXPECT_EQ(elem2->b, 2);
}

TEST(GetReady, AllElems)
{
	ImageFIFO fifo(sizeof(int), 10);
	std::vector<int*> elems;
	for (int i = 0; i < 10; ++i)
	{
		elems.push_back(reinterpret_cast<int*>(fifo.get_free()));
		ASSERT_TRUE(elems[i] != nullptr);
		*elems[i] = i;
	}

	for (int i = 0; i < 10; ++i)
	{
		fifo.add_ready(elems[i]);
		EXPECT_EQ(fifo.num_ready(), i + 1);
	}

	for (int i = 0; i < 10; ++i)
	{
		int* elem = reinterpret_cast<int*>(fifo.get_ready());
		EXPECT_EQ(elem, elems[i]);
		EXPECT_EQ(*elem, i);
		EXPECT_EQ(fifo.num_ready(), 10 - i - 1);
	}
}

TEST(GetReady, TooManyElems)
{
	ImageFIFO fifo(sizeof(char), 10);
	std::vector<char*> elems;
	for (int i = 0; i < 10; ++i)
	{
		elems.push_back(reinterpret_cast<char*>(fifo.get_free()));
		EXPECT_EQ(fifo.num_ready(), 0);
	}

	for (int i = 0; i < 10; ++i)
	{
		fifo.add_ready(elems[i]);
		EXPECT_EQ(fifo.num_ready(), i + 1);
	}

	for (int i = 0; i < 10; ++i)
	{
		EXPECT_EQ(reinterpret_cast<char*>(fifo.get_ready()), elems[i]);
		EXPECT_EQ(fifo.num_ready(), 10 - i - 1);
	}

	for (int i = 0; i < 10; ++i)
	{
		EXPECT_EQ(fifo.get_ready(), nullptr);
		EXPECT_EQ(fifo.num_ready(), 0);
	}
}

TEST(GetReady, RandomOrder)
{
	ImageFIFO fifo(sizeof(char), 10);
	std::vector<char*> elems;
	for (int i = 0; i < 10; ++i)
	{
		elems.push_back(reinterpret_cast<char*>(fifo.get_free()));
	}

	for (int i = 0; i < 5; ++i)
	{
		ASSERT_TRUE(elems[2 * i] != nullptr);
		*elems[2 * i] = 'a';
		fifo.add_ready(elems[2 * i]);
	}
	EXPECT_EQ(fifo.num_ready(), 5);

	for (int i = 0; i < 5; ++i)
	{
		ASSERT_TRUE(elems[2 * i + 1] != nullptr);
		*elems[2 * i + 1] = 'b';
		fifo.add_ready(elems[2 * i + 1]);
	}
	EXPECT_EQ(fifo.num_ready(), 10);

	for (int i = 0; i < 10; ++i)
	{
		char* elem = reinterpret_cast<char*>(fifo.get_ready());
		ASSERT_TRUE(elem != nullptr);
		if (i < 5)
		{
			EXPECT_EQ(*elem, 'a');
		}
		else
		{
			EXPECT_EQ(*elem, 'b');
		}
	}
}

TEST(AddFree, OneElem)
{
	ImageFIFO fifo(sizeof(int), 10);
	EXPECT_EQ(fifo.num_free(), 10);

	int* elem = reinterpret_cast<int*>(fifo.get_free());
	fifo.add_ready(elem);
	elem = reinterpret_cast<int*>(fifo.get_ready());
	EXPECT_EQ(fifo.num_free(), 9);

	fifo.add_free(elem);
	EXPECT_EQ(fifo.num_free(), 10);
}

TEST(AddFree, AllElems)
{
	ImageFIFO fifo(sizeof(char), 10);
	std::vector<void*> elems;

	for (int i = 0; i < 10; ++i)
	{
		elems.push_back(fifo.get_free());
		fifo.add_ready(elems[i]);
		elems[i] = fifo.get_ready();
	}

	EXPECT_EQ(fifo.num_free(), 0);
	EXPECT_EQ(fifo.num_busy(), 10);
	EXPECT_EQ(fifo.num_ready(), 0);

	for (int i = 0; i < 10; ++i)
	{
		fifo.add_free(elems[i]);
		EXPECT_EQ(fifo.num_free(), i + 1);
		EXPECT_EQ(fifo.num_busy(), 10 - i - 1);
	}
}

TEST(AddFree, ManyTimes)
{
	ImageFIFO fifo(sizeof(char), 10);
	fifo.add_ready(fifo.get_free());
	void* elem = fifo.get_ready();
	EXPECT_EQ(fifo.num_free(), 9);

	EXPECT_NO_THROW(fifo.add_free(elem));
	EXPECT_EQ(fifo.num_free(), 10);

	EXPECT_NO_THROW(fifo.add_free(elem));
	EXPECT_EQ(fifo.num_free(), 10);
}

TEST(AddFree, ElemNotFromQueue)
{
	ImageFIFO fifo(sizeof(char), 10);
	fifo.add_ready(fifo.get_free());
	fifo.get_ready();
	EXPECT_EQ(fifo.num_free(), 9);

	char elem;
	EXPECT_NO_THROW(fifo.add_ready((void*)(&elem)));
	EXPECT_EQ(fifo.num_free(), 9);
}

TEST(AddFree, RandomOrder)
{
	ImageFIFO fifo(sizeof(char), 10);
	std::vector<char*> elems;
	for (int i = 0; i < 10; ++i)
	{
		fifo.add_ready(fifo.get_free());
		elems.push_back(reinterpret_cast<char*>(fifo.get_ready()));
	}
	EXPECT_EQ(fifo.num_free(), 0);

	for (int i = 0; i < 5; ++i)
	{
		fifo.add_free(elems[8 - i * 2]);
		EXPECT_EQ(fifo.num_free(), i + 1);
	}

	for (int i = 0; i < 5; ++i)
	{
		fifo.add_free(elems[i * 2 + 1]);
		EXPECT_EQ(fifo.num_free(), i + 6);
	}
}

/* one thread with reader() и writer() */

template<typename T>
bool writer(ImageFIFO& fifo, T& elem)
{
	T* ptr = reinterpret_cast<T*>(fifo.get_free());
	if (ptr)
	{
		*ptr = elem;
		fifo.add_ready(ptr);
		return true;
	}
	return false;
}

template<typename T>
bool reader(ImageFIFO& fifo, T& elem)
{
	T* ptr = reinterpret_cast<T*>(fifo.get_ready());
	if (ptr)
	{
		elem = *ptr;
		fifo.add_free(ptr);
		return true;
	}
	return false;
}

TEST(OneThread, Small)
{
	size_t max_size = 10;
	ImageFIFO fifo(sizeof(int), max_size);
	std::vector<int> v1(max_size);
	std::vector<int> v2(max_size);
	for (size_t i = 0; i < max_size; ++i)
	{
		EXPECT_EQ(writer(fifo, v1[i]), true);
	}
	for (size_t i = 0; i < max_size; ++i)
	{
		EXPECT_EQ(reader(fifo, v2[i]), true);
	}
	EXPECT_EQ(v1, v2);
}

TEST(OneThread, Big)
{
	size_t max_size = 10;
	size_t num_elem = 100;

	ImageFIFO fifo(sizeof(int), max_size);
	std::vector<int> v1(num_elem);
	std::vector<int> v2(num_elem);
	for (size_t i = 0; i < 10; ++i)
	{
		for (size_t i = 0; i < max_size; ++i)
		{
			EXPECT_EQ(writer(fifo, v1[i]), true);
		}
		for (size_t i = 0; i < max_size; ++i)
		{
			EXPECT_EQ(reader(fifo, v2[i]), true);
		}
	}
	EXPECT_EQ(v1, v2);
}

/* two threads with reader() и writer() */

template <typename T>
void multith_writer(ImageFIFO& fifo, const std::vector<T>& test_v)
{
	size_t i = 0;
	while (i < test_v.size())
	{
		T* ptr = (T*)fifo.get_free();
		if (ptr)
		{
			*ptr = test_v[i];
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
void multith_reader(ImageFIFO& fifo, std::vector<T>& test_v, size_t max_size)
{
	size_t size = 0;
	size_t not_ready_count = 0;
	while (size < max_size)
	{
		T* ptr = (T*)fifo.get_ready();
		if (ptr)
		{
			test_v.push_back(*ptr);
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

TEST(MultiThread, Small)
{
	size_t max_size = 10;
	std::vector<int> v1(max_size, 10);
	std::vector<int> v2;
	ImageFIFO fifo(sizeof(int), max_size);
	std::thread t1(multith_writer<int>, std::ref(fifo), std::ref(v1));
	std::thread t2(multith_reader<int>, std::ref(fifo), std::ref(v2), max_size);
	t1.join();
	t2.join();
	EXPECT_EQ(v1, v2);
}

TEST(MultiThread, Big)
{
	size_t max_size = 10;
	size_t num_size = 100;
	std::vector<char> v1(num_size, 'a');
	std::vector<char> v2;
	ImageFIFO fifo(sizeof(char), max_size);
	std::thread t1(multith_writer<char>, std::ref(fifo), std::ref(v1));
	std::thread t2(multith_reader<char>, std::ref(fifo), std::ref(v2), num_size);
	t1.join();
	t2.join();
	EXPECT_EQ(v1, v2);
}