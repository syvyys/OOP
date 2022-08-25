#pragma once

#include <iostream>

template<typename T>
constexpr bool is_random_v = std::is_same_v<typename std::iterator_traits<T>::iterator_category(),
	std::random_access_iterator_tag()>;

template<typename T, typename U>
constexpr bool is_same_iteratror_type_v = std::is_same_v<typename std::iterator_traits<T>::value_type,
	typename std::iterator_traits<U>::value_type>;

template<typename T>
constexpr bool is_fundamental_iterator_type_v = std::is_fundamental_v<typename std::iterator_traits<T>::value_type>;

template <typename InputIter, typename OutputIter>
std::enable_if_t<
	is_random_v<InputIter>&&
	is_random_v<OutputIter>&&
	is_same_iteratror_type_v<InputIter, OutputIter>&&
	is_fundamental_iterator_type_v<InputIter>>
	optimized_copy(InputIter src_begin, InputIter src_end, OutputIter dst)
{
#ifdef _DEBUG
	std::cout << "optimized copy\n" << std::endl;
#endif // _DEBUG

	void* src_ptr = &(*src_begin);
	void* dst_ptr = &(*dst);
	size_t size = sizeof(*src_begin);
	size_t num = src_end - src_begin;
	memmove(dst_ptr, src_ptr, size * num);
}

template <typename InputIter, typename OutputIter>
std::enable_if_t<
	!is_random_v<InputIter> ||
	!is_random_v<OutputIter> ||
	!is_same_iteratror_type_v<InputIter, OutputIter> ||
	!is_fundamental_iterator_type_v<InputIter>>
	optimized_copy(InputIter src_begin, InputIter src_end, OutputIter dst)
{
#ifdef _DEBUG
	std::cout << "simple copy\n" << std::endl;
#endif // _DEBUG

	for (auto i = src_begin; i != src_end; ++i, ++dst)
	{
		*dst = *i;
	}
}