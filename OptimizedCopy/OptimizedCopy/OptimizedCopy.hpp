#pragma once

#include <iostream>

/* expressions for checking consitions */

template<typename T>
constexpr bool is_random_iterator_type =
std::is_same_v<typename std::iterator_traits<T>::iterator_category(), std::random_access_iterator_tag()>;

template<typename T, typename U>
constexpr bool is_same_iterator_type =
std::is_same_v<typename std::iterator_traits<T>::value_type, typename std::iterator_traits<U>::value_type>;

template<typename T>
constexpr bool is_fundamental_iterator_type =
std::is_fundamental_v<typename std::iterator_traits<T>::value_type>;

/* optimized case */

template<typename T, typename U>
constexpr bool optimized_case = is_same_iterator_type<T, U> &&
is_random_iterator_type<T> && is_random_iterator_type<U> &&
is_fundamental_iterator_type<T> && is_fundamental_iterator_type<U>;

template<typename InputIter, typename OutputIter>
std::enable_if_t<optimized_case<InputIter, OutputIter>>
	optimized_copy(InputIter src_begin, InputIter src_end, OutputIter dst)
{
#ifdef _DEBUG
	std::cout << "Optimized copy.\n" << std::endl;
#endif // _DEBUG

	void* src_ptr = &(*src_begin);
	void* dst_ptr = &(*dst);
	size_t size = sizeof(*src_begin);
	size_t num = src_end - src_begin;
	memmove(dst_ptr, src_ptr, size * num);
}

/* simple case */

template<typename T, typename U>
constexpr bool simple_case = !is_same_iterator_type<T, U> ||
!is_random_iterator_type<T> || !is_random_iterator_type<U> ||
!is_fundamental_iterator_type<T> || !is_fundamental_iterator_type<U>;

template <typename InputIter, typename OutputIter>
std::enable_if_t<simple_case<InputIter, OutputIter>>
	optimized_copy(InputIter src_begin, InputIter src_end, OutputIter dst)
{
#ifdef _DEBUG
	std::cout << "Simple copy.\n" << std::endl;
#endif // _DEBUG

	for (auto i = src_begin; i != src_end; ++i, ++dst)
	{
		*dst = *i;
	}
}