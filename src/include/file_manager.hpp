#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <curl/curl.h>
#include "extern\lodepng\lodepng.h"
#include "extern\nlohmann\single_include\nlohmann\json.hpp"
#include "utilities.hpp"

namespace file_manager
{
	void write_json_to_file(const nlohmann::ordered_json& object, const std::string& path, int indent = 4);
	nlohmann::ordered_json read_json_from_file(const std::string& path, const nlohmann::ordered_json& default_object);
	nlohmann::ordered_json read_json_from_web_page(const std::string& path, const std::vector<std::string>& headers = {});
	static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
	static size_t WriteOstream(void* buf, size_t size, size_t nmemb, void* userp);
	void add_lang_entry(const std::string& entry, const std::string& filename, std::string category);
	void make_directory(const std::string& path);
	std::string get_project_root();
	std::string get_bp_path();
	std::string get_rp_path();
	void set_bp_path(const std::string& path);
	void set_rp_path(const std::string& path);
	std::vector<std::string> get_files_in_directory(const std::string& path, const bool& use_dir_path_if_invalid = false);
	std::vector<std::string> get_files_in_directory(const std::string& path, std::vector<std::string> filters);
	std::string get_file_in_directory(const std::string& dir_path, const std::string& filename);
	void write_file(const std::string& path, const std::string& contents);
	std::string read_file(const std::string& path);
	std::string read_string_from_web_page(const std::string& path, const std::vector<std::string>& headers = {});
	void write_file_from_web_page(const std::string& url, const std::string& filepath, const std::vector<std::string>& headers = {});
	void write_blank_png(const std::string& path, unsigned width = 16, unsigned height = 16, const bool overwrite = false);
	std::vector<std::string> get_headers();
}