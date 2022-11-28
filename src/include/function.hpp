#pragma once
#include "extern\cxxopts\include\cxxopts.hpp"
#include "extern\nlohmann\single_include\nlohmann\json.hpp"
#include "help.hpp"
#include "file_manager.hpp"
#include "utilities.hpp"

namespace function
{
	void new_function(int argc, char* argv[]);
	void format_function(std::string& function);
}
