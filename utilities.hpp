#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iostream>

namespace utilities
{
	std::vector<std::string> split(const std::string& val, const char& deliminator = ' ');
	std::vector<std::string> split(std::string val, const std::string& deliminator = " ");

	template<class T>
	void push_back_if(std::vector<T>& vector, const T& val, const bool& condition);
}