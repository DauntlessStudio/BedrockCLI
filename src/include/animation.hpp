#pragma once
#include "extern\cxxopts\include\cxxopts.hpp"
#include "extern\nlohmann\single_include\nlohmann\json.hpp"
#include "help.hpp"
#include "file_manager.hpp"
#include "utilities.hpp"

namespace animation
{
	void new_animation(int argc, char* argv[]);
	void new_controller_from_args(int argc, char* argv[]);
	void new_controller(std::vector<std::string> entries, std::vector<std::string> exits, std::vector<std::string> anims, std::string return_query, std::string default_query, std::string state_name, std::string controller_name, int indent = 4);
}
