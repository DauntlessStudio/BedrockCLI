#pragma once
#include <string_view>
#include <vector>
#include <functional>
#include <algorithm>
#include <map>
#include "help.hpp"
#include "item.hpp"
#include "block.hpp"
#include "entity.hpp"
#include "animation.hpp"
#include <stdexcept>
#include <iostream>
#include "extern\cxxopts\include\cxxopts.hpp"

namespace program_arguments
{
	void parse(int argc, char* argv[]);
	void run_command(int argc, char* argv[]);
	void assign_command(const std::string& arg);
	std::string get_options_help();
}