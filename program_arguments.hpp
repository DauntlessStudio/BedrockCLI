#pragma once
#include <string_view>
#include <vector>
#include <functional>
#include <algorithm>
#include <map>
#include "help.hpp"
#include <stdexcept>

namespace program_arguments
{

	void parse(int argc, char* argv[]);
	void run_command();
	void assign_command(const std::string_view& arg, const int command_index);
}