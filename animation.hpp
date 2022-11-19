#pragma once
#include "extern\cxxopts\include\cxxopts.hpp"
#include "extern\nlohmann\single_include\nlohmann\json.hpp"
#include "help.hpp"
#include "file_manager.hpp"
#include "utilities.hpp"

namespace animation
{
	void new_animation(int argc, char* argv[]);
}
