#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

namespace utilities
{
	std::vector<std::string> split(const std::string& val, const char& deliminator = ' ');
}