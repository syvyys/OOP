#include <list>
#include <array>
#include <deque>
#include <vector>
#include <forward_list>
#include "gtest/gtest.h"
#include "../OptimizedCopy/OptimizedCopy.hpp"

TEST(StaticArray, Int)
{
	int src[5] = { 1, 2, 3, 4, 5 };
	int dst[5] = { 0 };
	optimized_copy(std::begin(src), std::end(src), std::begin(dst));
	for (int i = 0; i < 5; ++i)
	{
		EXPECT_EQ(src[i], dst[i]);
	}
}

TEST(StaticArray, Char)
{
	char src[5] = { 'a', 'b', 'c', 'd', 'e' };
	char dst[5] = { 0 };
	optimized_copy(std::begin(src), std::end(src), std::begin(dst));
	for (int i = 0; i < 5; ++i)
	{
		EXPECT_EQ(src[i], dst[i]);
	}
}

TEST(StaticArray, String)
{
	std::string src[5] = { "abc", "def", "E=mc^2", "Jesus", "he-he-he" };
	std::string dst[5] = { "", "", "", "", "" };
	optimized_copy(std::begin(src), std::end(src), std::begin(dst));
	for (int i = 0; i < 5; ++i)
	{
		EXPECT_EQ(src[i], dst[i]);
	}
}

TEST(Vector, Int)
{
	std::vector<int> src = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	std::vector<int> dst(10);
	optimized_copy(src.begin(), src.end(), dst.begin());
	EXPECT_EQ(src, dst);
}

TEST(Vector, Char)
{
	std::vector<char> src = { 'a', 'a', 'b', 'b', 'c', 'c', 'd', 'd', 'e', 'e' };
	std::vector<char> dst(10);
	optimized_copy(src.begin(), src.end(), dst.begin());
	EXPECT_EQ(src, dst);
}

TEST(Vector, String)
{
	std::vector<std::string> src = { "hello", "world", "!", "text", "program" };
	std::vector<std::string> dst(5);
	optimized_copy(src.begin(), src.end(), dst.begin());
	EXPECT_EQ(src, dst);
}

TEST(Vector, VectorInt)
{
	std::vector<int> v1 = { 1, 2, 3, 5, 6, 7, 8, 9, 10 };
	std::vector<int> v2 = { 10, 10, 10 };
	std::vector<int> v3 = { 0 };
	std::vector<int> v4 = { 152456325, -1, -2 };
	std::vector<int> v5 = { 1000, 1000 };
	std::vector<std::vector<int>> src = { v1, v2, v3, v4, v5 };
	std::vector<std::vector<int>> dst(5);
	optimized_copy(src.begin(), src.end(), dst.begin());
	EXPECT_EQ(src, dst);
}

TEST(Vector, IntToBackinserted)
{
	std::vector<int> src = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	std::vector<int> dst;
	optimized_copy(src.begin(), src.end(), std::back_inserter(dst));
	EXPECT_EQ(src, dst);
}

TEST(String, CopyEntirely)
{
	std::string src =
		"Our God is an Awsome God\n\
		He reigns from the heaven above\n\
		With wisdom, power and love\n\
		Our God is an Awsome Gode\n\
		\n\
		When He rolls up His sleeves\n\
		He ain't just puttin' on the Ritz\n\
		(Our God is an Awsome God)\n\
		There is thunder in His footsteps\n\
		And lightning in His fists\n\
		(Our God is an Awsome God)\n\
		Well the Lord he wasn't jokin'\n\
		When He kicked 'em out of Eden\n\
		It wasn't for no reason that He shed His blood\n\
		His return is very close and so you better be believin'\n\
		That our God is an Awsome God\n\
		\n\
		Our God is an Awsome God\n\
		He reigns from the heaven above\n\
		With wisdom, power and love\n\
		Our God is an Awsome Gode";

	std::string dst;
	for (int i = 0; i < 1000; ++i)
	{
		dst.push_back('0');
	}

	optimized_copy(src.begin(), src.end(), dst.begin());

	for (size_t i = 0; i < src.size(); ++i)
	{
		EXPECT_EQ(src[i], dst[i]);
	}

	for (size_t i = src.size(); i < dst.size(); ++i)
	{
		EXPECT_EQ(dst[i], '0');
	}
}

TEST(String, CopyPartly)
{
	std::string src =
		"Our God is an Awsome God\n\
		He reigns from the heaven above\n\
		With wisdom, power and love\n\
		Our God is an Awsome Gode\n\
		\n\
		When He rolls up His sleeves\n\
		He ain't just puttin' on the Ritz\n\
		(Our God is an Awsome God)\n\
		There is thunder in His footsteps\n\
		And lightning in His fists\n\
		(Our God is an Awsome God)\n\
		Well the Lord he wasn't jokin'\n\
		When He kicked 'em out of Eden\n\
		It wasn't for no reason that He shed His blood\n\
		His return is very close and so you better be believin'\n\
		That our God is an Awsome God\n\
		\n\
		Our God is an Awsome God\n\
		He reigns from the heaven above\n\
		With wisdom, power and love\n\
		Our God is an Awsome Gode";
	std::string dst;
	for (int i = 0; i < 100; ++i)
	{
		dst.push_back('0');
	}

	optimized_copy(src.begin() + 100, src.begin() + 200, dst.begin());

	for (int i = 0; i < 100; ++i)
	{
		EXPECT_EQ(src[i + 100], dst[i]);
	}
}

TEST(DifferentContainers, ListToVector)
{
	std::list<int> src = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	std::vector<int> dst(10);
	optimized_copy(src.begin(), src.end(), dst.begin());
	auto l = src.begin();
	auto v = dst.begin();
	for (; l != src.end(); ++l, ++v)
	{
		EXPECT_EQ(*l, *v);
	}
}

TEST(DifferentContainers, ArrayToVector)
{
	int src[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	std::vector<int> dst(10);
	optimized_copy(std::begin(src), std::end(src), dst.begin());
	auto a = std::begin(src);
	auto v = dst.begin();
	for (; a != std::end(src); ++a, ++v)
	{
		EXPECT_EQ(*a, *v);
	}
}

TEST(DifferentContainers, StringToVector)
{
	std::string src = "aaaaaaaaa";
	std::vector<char> dst(10);
	optimized_copy(src.begin(), src.end(), dst.begin());
	for (int i = 0; i < 10; ++i)
	{
		EXPECT_EQ(src[i], dst[i]);
	}
}

TEST(DifferentContainers, DequeToForwardList)
{
	std::deque<int> src = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	std::forward_list<int> dst(10);
	optimized_copy(src.begin(), src.end(), dst.begin());
	auto d = src.begin();
	auto l = dst.begin();
	for (; d != src.end(); ++d, ++l)
	{
		EXPECT_EQ(*d, *l);
	}
}

TEST(DifferentTypes, IntToLong)
{
	std::vector<int> src = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	std::vector<long> dst(10);
	optimized_copy(src.begin(), src.end(), dst.begin());
	for (int i = 0; i < 10; ++i)
	{
		EXPECT_EQ(src[i], dst[i]);
	}
}

TEST(DifferentTypes, CharToInt)
{
	std::array<char, 10> src = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j' };
	std::array<int, 10> dst;
	optimized_copy(std::begin(src), std::end(src), std::begin(dst));
	for (int i = 0; i < 10; ++i)
	{
		EXPECT_EQ(src[i], dst[i]);
	}
}