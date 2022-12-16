#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <Windows.h>

#define YELLOW utilities::color(6)
#define CYAN utilities::color(3)
#define RED utilities::color(4)
#define GREEN utilities::color(2)
#define WHITE utilities::color(7)

namespace utilities
{
	std::vector<std::string> split(const std::string& val, const char& deliminator = ' ');
	std::vector<std::string> split(std::string val, const std::string& deliminator = " ");
	void replace_all(std::string& str, const std::string& from, const std::string& to);
	std::string to_upper(const std::string& str);
	std::string to_lower(const std::string& str);
	std::string format_name(const std::string& str);
	std::string vector_to_string(const std::vector<std::string>& vec);
	std::string color(int color_code);

	template<class T>
	void push_back_if(std::vector<T>& vector, const T& val, const bool& condition);

	struct name
	{
	public:
		name();
		name(const std::string input);
		std::string dir;
		std::string space;
		std::string shortname;
		std::string filename;
		std::string internal_name;
	};
}