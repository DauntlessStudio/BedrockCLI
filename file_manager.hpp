#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include "extern\nlohmann\single_include\nlohmann\json.hpp"
#include "utilities.hpp"

namespace file_manager
{
	void write_json_to_file(const nlohmann::ordered_json& object, const std::string& path, int indent = 4);
	nlohmann::ordered_json read_json_from_file(const std::string& path, const nlohmann::ordered_json& default_object);
	void add_lang_entry(const std::string& entry, const std::string& filename, std::string category);
	void make_directory(const std::string& path);
	std::string get_project_root();
	std::string get_bp_path();
	std::string get_rp_path();
	void set_bp_path(const std::string& path);
	void set_rp_path(const std::string& path);
	std::vector<std::string> get_files_in_directory(const std::string& path);
	std::vector<std::string> get_files_in_directory(const std::string& path, std::vector<std::string> filters);
	std::string get_file_in_directory(const std::string& dir_path, const std::string& filename);
	void write_file(const std::string& path, const std::string& contents);
}