#pragma once

#include <iostream>
#include "SumAllTraits.hpp"

/* Not container, single element */

template <typename T, std::enable_if_t<std::is_arithmetic_v<T> || std::is_same_v<T, std::string>, int> = 0>
auto SumAll(const T& v)
{
	return v;
}

/* Sequence container */

template <typename T, std::enable_if_t<(std::is_arithmetic_v<typename T::value_type> &&
	!std::is_same_v<T, std::string>) || std::is_same_v<typename T::value_type, std::string>, int> = 0>
auto SumAll(const T& v)
{
	typename SumAllTraits<typename T::value_type>::SumType sum = {};
	for (auto i : v)
	{
		sum += i;
	}
	return sum;
}

/* Associative container */

template <typename T, std::enable_if_t<std::is_arithmetic_v<typename T::mapped_type> ||
	std::is_same_v<typename T::mapped_type, std::string>, int> = 0>
auto SumAll(const T& v)
{
	typename SumAllTraits<typename T::mapped_type>::SumType sum = {};
	for (auto& i : v)
	{
		sum += i.second;
	}
	return sum;
}