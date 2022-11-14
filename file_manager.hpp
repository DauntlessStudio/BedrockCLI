#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include "extern\nlohmann\single_include\nlohmann\json.hpp"

namespace file_manager
{
	void write_json_to_file(const nlohmann::ordered_json& object, const std::string& path, int indent = 4);
	nlohmann::ordered_json read_json_from_file(const std::string& path, const nlohmann::ordered_json& default_object);
	void make_directory(const std::string& path);
	std::string get_project_root();
	std::string get_bp_path();
	std::string get_rp_path();
}