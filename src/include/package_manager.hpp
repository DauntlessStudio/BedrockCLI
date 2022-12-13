#pragma once
#include "extern\cxxopts\include\cxxopts.hpp"
#include "extern\nlohmann\single_include\nlohmann\json.hpp"
#include "help.hpp"
#include "file_manager.hpp"
#include "utilities.hpp"
#include <regex>

namespace package_manager
{
	void import_package(int argc, char* argv[]);

	void recursive_download(const std::string& path);
}