#pragma once

#include <string>

template<typename T>
class SumAllTraits;

// STRING

template<>
class SumAllTraits<std::string>
{
public:
	using SumType = std::string;
};

// CHAR

template<>
class SumAllTraits<char>
{
public:
	using SumType = std::string;
};

template<>
class SumAllTraits<signed char>
{
public:
	using SumType = std::string;
};

template<>
class SumAllTraits<unsigned char>
{
public:
	using SumType = unsigned short;
};

// SHORT

template<>
class SumAllTraits<short>
{
public:
	using SumType = int;
};

template<>
class SumAllTraits<unsigned short>
{
public:
	using SumType = unsigned int;
};

// INT

template<>
class SumAllTraits<int>
{
public:
	using SumType = long long;
};

template<>
class SumAllTraits<unsigned int>
{
public:
	using SumType = unsigned long long;
};

// LONG

template<>
class SumAllTraits<long>
{
public:
	using SumType = long long;
};

template<>
class SumAllTraits<unsigned long>
{
public:
	using SumType = unsigned long long;
};

// LONG LONG

template<>
class SumAllTraits<long long>
{
public:
	using SumType = long long;
};

template<>
class SumAllTraits<unsigned long long>
{
public:
	using SumType = unsigned long long;
};

// FLOAT, DOUBLE, LONG DOUBLE

template<>
class SumAllTraits<float>
{
public:
	using SumType = double;
};

template<>
class SumAllTraits<double>
{
public:
	using SumType = long double;
};

template<>
class SumAllTraits<long double>
{
public:
	using SumType = long double;
};