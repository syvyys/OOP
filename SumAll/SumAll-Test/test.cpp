#include "gtest/gtest.h"
#include "../SumAll/SumAll.hpp"

#include <map>
#include <set>
#include <list>
#include <string>
#include <vector>
#include <numeric>
#include <iostream>
#include <forward_list>
#include <unordered_set>
#include <unordered_map>

TEST(NotContainer, String)
{
	std::string input = "abcd";
	auto output = SumAll(input);
	EXPECT_EQ(input, output);
}

TEST(NotContainer, Char)
{
	char input = 'a';
	auto output = SumAll(input);
	EXPECT_EQ(input, output);
}

TEST(NotContainer, SignedChar)
{
	signed char input = 'a';
	auto output = SumAll(input);
	EXPECT_EQ(input, output);
}

TEST(NotContainer, UnsignedChar)
{
	unsigned char input = 100;
	auto output = SumAll(input);
	EXPECT_EQ(input, output);
}

TEST(NotContainer, Short)
{
	short input = -100;
	auto output = SumAll(input);
	EXPECT_EQ(input, output);
}

TEST(NotContainer, UnsignedShort)
{
	unsigned short input = 100;
	auto output = SumAll(input);
	EXPECT_EQ(input, output);
}

TEST(NotContainer, Int)
{
	int input = -100;
	auto output = SumAll(input);
	EXPECT_EQ(input, output);
}

TEST(NotContainer, UnsignedInt)
{
	unsigned int input = 100;
	auto output = SumAll(input);
	EXPECT_EQ(input, output);
}

TEST(NotContainer, Long)
{
	long input = -1000000;
	auto output = SumAll(input);
	EXPECT_EQ(input, output);
}

TEST(NotContainer, UnsignedLong)
{
	unsigned long input = 1000000;
	auto output = SumAll(input);
	EXPECT_EQ(input, output);
}

TEST(NotContainer, LongLong)
{
	long long input = -100000000000000000;
	auto output = SumAll(input);
	EXPECT_EQ(input, output);
}

TEST(NotContainer, UnsignedLongLong)
{
	unsigned long long input = 100000000000000000;
	auto output = SumAll(input);
	EXPECT_EQ(input, output);
}

TEST(NotContainer, Float)
{
	float input = 1.5;
	auto output = SumAll(input);
	EXPECT_EQ(input, output);
}

TEST(NotContainer, Double)
{
	double input = 1.5201;
	auto output = SumAll(input);
	EXPECT_EQ(input, output);
}

TEST(NotContainer, LongDouble)
{
	long double input = 1000.52;
	auto output = SumAll(input);
	EXPECT_EQ(input, output);
}

TEST(Vector, String)
{
	std::vector<std::string> input = { "Hello", " ", "World", "!" };
	std::string expected = "Hello World!";
	auto output = SumAll(input);
	EXPECT_EQ(output, expected);
}

TEST(Vector, Char)
{
	std::vector<char> input = { 'a', 'b', 'c', 'd', 'e', 'f' };
	std::string expected = "abcdef";
	auto output = SumAll(input);
	EXPECT_EQ(output, expected);
}

TEST(Vector, Short)
{
	std::vector<short> input = { 10000, 10000, 10000, 10000 };
	int expected = 40000;
	auto output = SumAll(input);
	EXPECT_EQ(output, expected);
}

TEST(Vector, Int)
{
	std::vector<int> input = { std::numeric_limits<int>::max(), 100, 100, 100 };
	constexpr long long expected = static_cast<long long>(std::numeric_limits<int>::max()) + 300;
	auto output = SumAll(input);
	EXPECT_EQ(output, expected);
}

TEST(Vector, Long)
{
	std::vector<long> input = { std::numeric_limits<long>::max(), 10000, 10000, 10000, 10000 };
	constexpr long long expected = static_cast<long long>(std::numeric_limits<long>::max()) + 40000;
	auto output = SumAll(input);
	EXPECT_EQ(output, expected);
}

TEST(Vector, LongLong)
{
	std::vector<long long> input = { 10000, 10000, 10000, 10000, 10000 };
	long long expected = 50000;
	auto output = SumAll(input);
	EXPECT_EQ(output, expected);
}

TEST(Vector, Float)
{
	std::vector<float> input = { 0.5, 9.5, 100.0, 100.0, 100.0 };
	double expected = 310.0;
	auto output = SumAll(input);
	EXPECT_EQ(output, expected);
}

TEST(Vector, Double)
{
	std::vector<double> input = { 0.55, 9.45, 100.05, 100.0, 100.0 };
	long double expected = 310.05;
	auto output = SumAll(input);
	EXPECT_EQ(output, expected);
}

TEST(Vector, LongDouble)
{
	std::vector<long double> input = { 0.55, 9.45, 100.05, 100.0, 100.0 };
	auto output = SumAll(input);
	EXPECT_EQ(output, 310.05);
}

TEST(List, Char)
{
	std::list<char> input = { 'a', 'b', 'c', 'd', 'e', 'f' };
	std::string expected = "abcdef";
	std::string output = SumAll(input);
	EXPECT_EQ(output, expected);
}

TEST(List, Short)
{
	std::list<short> input = { 0, 10, 100, 200 };
	int expected = 310;
	auto output = SumAll(input);
	EXPECT_EQ(output, expected);
}

TEST(List, Int)
{
	std::list<int> input = { 100, 100, 100, 100 };
	long expected = 400;
	auto output = SumAll(input);
	EXPECT_EQ(output, expected);
}

TEST(List, Long)
{
	std::list<long> input = { 10000, 10000, 10000, 10000, 10000 };
	long long expected = 50000;
	auto output = SumAll(input);
	EXPECT_EQ(output, expected);
}

TEST(ForwardList, String)
{
	std::forward_list<std::string> input = { "No ", "more ", "tests." };
	std::string expected = "No more tests.";
	auto output = SumAll(input);
	EXPECT_EQ(output, expected);
}

TEST(ForwardList, Char)
{
	std::forward_list<char> input = { 'q', 'w', 'e', 'r' };
	std::string expected = "qwer";
	auto output = SumAll(input);
	EXPECT_EQ(output, expected);
}

TEST(ForwardList, SignedChar)
{
	std::forward_list<signed char> input = { 'a', 'b', 'c', 'd' };
	std::string expected = "abcd";
	auto output = SumAll(input);
	EXPECT_EQ(output, expected);
}

TEST(ForwardList, UnsignedChar)
{
	std::forward_list<unsigned char> input = { 250, 250, 250, 250 };
	int expected = 1000;
	auto output = SumAll(input);
	EXPECT_EQ(output, expected);
}

TEST(ForwardList, Int)
{
	std::forward_list<int> input = { 1000000, 1000000, 1000000, 1000000 };
	long expected = 4000000;
	auto output = SumAll(input);
	EXPECT_EQ(output, expected);
}

TEST(Set, String)
{
	std::set<std::string> input = { "abc", "def", "ghi" };
	std::string expected = "abcdefghi";
	auto output = SumAll(input);
	EXPECT_EQ(output, expected);
}

TEST(Set, Char)
{
	std::set<char> input = { 'a', 'b', 'c' };
	std::string expected = "abc";
	auto output = SumAll(input);
	EXPECT_EQ(output, expected);
}

TEST(Set, Float)
{
	std::set<float> input = { 10000.1f, 10000.2f, 10000.3f, 10000.4f };
	double expected = 40001.0;
	auto output = SumAll(input);
	EXPECT_EQ(output, expected);
}

TEST(Set, Double)
{
	std::set<double> input = { 10000.11, 10000.12, 10000.13 };
	long double expected = 30000.36;
	auto output = SumAll(input);
	EXPECT_EQ(output, expected);
}

TEST(Set, LongDouble)
{
	std::set<long double> input = { 1000000000.111, 1000000000.112, 1000000000.115, 1000000000.11 };
	long double expected = 4000000000.448;
	auto output = SumAll(input);
	EXPECT_EQ(output, expected);
}

TEST(UnorderedSet, Short)
{
	std::set<short> input = { 100, 101, 102, 103 };
	int expected = 406;
	auto output = SumAll(input);
	EXPECT_EQ(output, expected);
}

TEST(UnorderedSet, SignedShort)
{
	std::set<signed short> input = { 100, -100, 101, -101 };
	signed int expected = 0;
	auto output = SumAll(input);
	EXPECT_EQ(output, expected);
}

TEST(UnorderedSet, UnsignedShort)
{
	std::set<unsigned short> input = { 100, 101, 99, 200 };
	unsigned int expected = 500;
	auto output = SumAll(input);
	EXPECT_EQ(output, expected);
}

TEST(Map, String)
{
	std::map<int, std::string> input = { {99, "Hello "}, {100, "World"} };
	std::string expected = "Hello World";
	auto output = SumAll(input);
	EXPECT_EQ(output, expected);
}

TEST(Map, Int)
{
	std::map<int, int> input = { {100, 101}, {99, 200} };
	long long expected = 301;
	auto output = SumAll(input);
	EXPECT_EQ(output, expected);
}

TEST(Map, SignedInt)
{
	std::map<int, signed int> input = { {100, -101}, {99, 200} };
	long long expected = 99;
	auto output = SumAll(input);
	EXPECT_EQ(output, expected);
}

TEST(Map, UnsignedInt)
{
	std::map<int, unsigned int> input = { {100, 100}, {99, 200} };
	unsigned long long expected = 300;
	auto output = SumAll(input);
	EXPECT_EQ(output, expected);
}

TEST(UnorderedMap, Long)
{
	std::unordered_map<char, long> input = { {'a', 100000}, {'b', 200000} };
	long long expected = 300000;
	auto output = SumAll(input);
	EXPECT_EQ(output, expected);
}

TEST(UnorderedMap, SignedLong)
{
	std::unordered_map<char, signed long> input = { {'a', -100000}, {'b', 200000} };
	long long expected = 100000;
	auto output = SumAll(input);
	EXPECT_EQ(output, expected);
}

TEST(UnorderedMap, UnsignedLong)
{
	std::unordered_map<char, unsigned long> input = { {'a', 200000}, {'b', 200000} };
	long long expected = 400000;
	auto output = SumAll(input);
	EXPECT_EQ(output, expected);
}