#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <forward_list>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <numeric>
#include "SumAllTraits.hpp"
#include "SumAll.hpp"

int main()
{
	std::vector<int> v = { 1, 2, 3 };
	std::list<int> l = { 1, 2, 3 };
	std::forward_list<int> fl = { 1, 2, 3 };
	std::set<short> s = { 1, 2, 3 };
	std::unordered_set<int> us = { 1, 2, 3 };
	std::unordered_map<int, double> m = { {1,1.1}, {2,2} };

	auto num = SumAll(m);
	std::cout << num << std::endl;
}