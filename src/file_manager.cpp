#include "include/file_manager.hpp"

namespace
{
	std::string project_root;
	std::string behavior_pack;
	std::string resource_pack;
	std::string bp_subname = "behavior_packs";
	std::string rp_subname = "resource_packs";
	std::vector<std::string> curl_headers;
}

void file_manager::write_json_to_file(const nlohmann::ordered_json& object, const std::string& path, int indent)
{
	make_directory(path);
	std::ofstream output(path);

	try
	{
		output << std::setw(indent) << object;
	}
	catch (const std::exception& e)
	{
		std::cerr << RED << "Error writing JSON at: " << path << std::endl << e.what() << WHITE << std::endl;
	}

	std::cout << GREEN << "Saved JSON at: " << path << WHITE << std::endl;
	output.close();
}

nlohmann::ordered_json file_manager::read_json_from_file(const std::string& path, const nlohmann::ordered_json& default_object)
{
	if (!std::filesystem::exists(path))
	{
		return default_object;
	}

	nlohmann::ordered_json object;
	std::ifstream input;
	input.open(path);

	try
	{
		object = nlohmann::ordered_json::parse(input, nullptr, true, true);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	input.close();
	return object;
}

nlohmann::ordered_json file_manager::read_json_from_web_page(const std::string& path, const std::vector<std::string>& headers)
{
	nlohmann::ordered_json object;

	/* Parse to JSON */
	try
	{
		object = nlohmann::ordered_json::parse(read_string_from_web_page(path, headers), nullptr, true, true);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return object;
}

size_t file_manager::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

size_t file_manager::WriteOstream(void* buf, size_t size, size_t nmemb, void* userp)
{
	if (userp)
	{
		std::ostream& os = *static_cast<std::ostream*>(userp);
		std::streamsize len = size * nmemb;
		if (os.write(static_cast<char*>(buf), len))
			return len;
	}

	return 0;
}

void file_manager::add_lang_entry(const std::string& entry, const std::string& filename, std::string category)
{
	std::string path = get_rp_path() + "\\texts\\" + filename + ".lang";
	make_directory(path);
	category = utilities::to_upper(category);

	std::ifstream file(path);
	std::stringstream f_stream;
	f_stream << file.rdbuf();
	std::string f_string = f_stream.str();
	std::vector<std::string> lines = utilities::split(f_string, '\n');

	size_t index = 0;
	bool found_category = false;
	for (auto& line : lines)
	{
		if (line.find(category) != std::string::npos)
		{
			found_category = true;
			break;
		}
		index++;
	}

	if (found_category)
	{
		std::vector<std::string>::iterator it = lines.begin() + index;
		lines.insert(it + 1, entry);
	}
	else
	{
		lines.push_back("## " + category + " " + std::string(100, '='));
		lines.push_back(entry);
	}

	file.close();

	std::ofstream output(path, std::ios::trunc);

	for (size_t i = 0; i < lines.size(); i++)
	{
		output << lines[i] << std::endl;
	}
	output.close();

	std::cout << GREEN << "Added line to lang at: " << path << WHITE << std::endl;
}

void file_manager::make_directory(const std::string& path)
{
	size_t index = path.find_last_of('\\');
	std::string tmp = path.substr(0, index + 1);
	std::filesystem::path dir(tmp);
	if (!std::filesystem::exists(dir))
	{
		std::cout << YELLOW << "Directory Doesn't Exist. Creating..." << WHITE << std::endl;
		std::filesystem::create_directories(dir);
	}
}

std::string file_manager::get_project_root()
{
	if (!project_root.empty())
	{
		return project_root;
	}

	std::string working_directory = std::filesystem::current_path().u8string();
	std::size_t index = working_directory.find("world_template");
	if (index != std::string::npos)
	{
		project_root = working_directory.substr(0, index + 15);
		return working_directory.substr(0, index);
	}

	char* buf = nullptr;
	size_t sz = 0;
	if (_dupenv_s(&buf, &sz, "USERPROFILE") == 0 && buf != nullptr)
	{
		std::string path = std::string(buf) + "\\AppData\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\LocalState\\games\\com.mojang";
		free(buf);
		bp_subname = "development_behavior_packs";
		rp_subname = "development_resource_packs";
		project_root = path;
		return path;
	}
	return working_directory;
}

std::string file_manager::get_bp_path()
{
	if (!behavior_pack.empty())
	{
		return behavior_pack;
	}

	if (std::filesystem::exists(get_project_root()))
	{
		for (auto const& dir_entry : std::filesystem::directory_iterator(get_project_root() + "behavior_packs"))
		{
			behavior_pack = dir_entry.path().u8string();
			return behavior_pack;
		}
	}
	return std::string();
}

std::string file_manager::get_rp_path()
{
	if (!resource_pack.empty())
	{
		return resource_pack;
	}

	if (std::filesystem::exists(get_project_root()))
	{
		for (auto const& dir_entry : std::filesystem::directory_iterator(get_project_root() + "resource_packs"))
		{
			resource_pack = dir_entry.path().u8string();
			return resource_pack;
		}
	}
	return std::string();
}

void file_manager::set_bp_path(const std::string& path)
{
	behavior_pack = path;
	utilities::replace_all(behavior_pack, "/", "\\");
}

void file_manager::set_rp_path(const std::string& path)
{
	resource_pack = path;
	utilities::replace_all(resource_pack, "/", "\\");
}

std::vector<std::string> file_manager::get_files_in_directory(const std::string& path, const bool& use_dir_path_if_invalid)
{
	if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path))
	{
		return use_dir_path_if_invalid ? std::vector<std::string>(1, path) : std::vector<std::string>();
	}

	std::vector<std::string> files;
	for (auto const& file : std::filesystem::recursive_directory_iterator(path))
	{
		if (file.is_regular_file())
		{
			files.push_back(file.path().u8string());
		}
	}
	return files;
}

std::vector<std::string> file_manager::get_files_in_directory(const std::string& path, std::vector<std::string> filters)
{
	filters.erase(std::remove(filters.begin(), filters.end(), ""), filters.end());

	if (filters.size() == 0)
	{
		return get_files_in_directory(path);
	}

	if (!std::filesystem::exists(path))
	{
		return std::vector<std::string>();
	}

	std::vector<std::string> files;
	for (auto const& file : std::filesystem::recursive_directory_iterator(path))
	{
		if (file.is_regular_file())
		{
			std::string f_string = file.path().u8string();
			for (auto const& filter : filters)
			{
				if (utilities::split(f_string, '\\').back() == utilities::split(filter, '\\').back())
				{
					files.push_back(f_string);
					break;
				}
			}
		}
	}
	return files;
}

std::string file_manager::get_file_in_directory(const std::string& dir_path, const std::string& filename)
{
	if (!std::filesystem::exists(dir_path))
	{
		return std::string();
	}

	for (auto const& file : std::filesystem::recursive_directory_iterator(dir_path))
	{
		std::string f_string = file.path().u8string();
		if (utilities::split(f_string, '\\').back() == utilities::split(filename, '\\').back())
		{
			return f_string;
		}
	}

	return std::string();
}

void file_manager::write_file(const std::string& path, const std::string& contents)
{
	make_directory(path);
	std::ofstream output(path);

	output << contents;

	output.close();

	std::cout << GREEN << "Wrote: " << path << WHITE << std::endl;
}

std::string file_manager::read_file(const std::string& path)
{
	if (!std::filesystem::exists(path))
	{
		return std::string();
	}

	std::ifstream file(path);
	std::stringstream f_stream;
	f_stream << file.rdbuf();
	std::string f_string = f_stream.str();

	return f_string;
}

std::string file_manager::read_string_from_web_page(const std::string& path, const std::vector<std::string>& headers)
{
	CURL* curl;
	CURLcode res;
	std::string readBuffer;

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if (curl)
	{
		struct curl_slist* curl_headers = NULL;
		curl_easy_setopt(curl, CURLOPT_URL, path.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

		/* Handle headers */
		for (const auto& header : headers)
		{
			curl_headers = curl_headers = curl_slist_append(curl_headers, header.c_str());
		}
		if (headers.size() > 0)
		{
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curl_headers);
		}

		res = curl_easy_perform(curl);
		curl_slist_free_all(curl_headers);

		/* Check for errors */
		if (res != CURLE_OK)
		{
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));
			return "";
		}

		if (readBuffer.empty())
		{
			std::cout << "Response Was Empty..." << std::endl;
			return "";
		}

		/* always cleanup */
		curl_easy_cleanup(curl);
	}

	return readBuffer;
}

void file_manager::write_file_from_web_page(const std::string& url, const std::string& filepath, const std::vector<std::string>& headers)
{
	make_directory(filepath);

	CURL* curl;
	CURLcode res;
	std::ofstream write_stream(filepath, std::ostream::binary);

	std::cout << GREEN << "Writing to: " << filepath << WHITE << std::endl;

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if (curl)
	{
		struct curl_slist* curl_headers = NULL;
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteOstream);
		curl_easy_setopt(curl, CURLOPT_FILE, &write_stream);

		/* Handle headers */
		for (const auto& header : headers)
		{
			curl_headers = curl_headers = curl_slist_append(curl_headers, header.c_str());
		}
		if (headers.size() > 0)
		{
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curl_headers);
		}

		res = curl_easy_perform(curl);
		curl_slist_free_all(curl_headers);

		/* Check for errors */
		if (res != CURLE_OK)
		{
			RED;
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));
			WHITE;
			return;
		}

		if (write_stream.bad())
		{
			std::cout << RED << "Invalid Write..." << WHITE << std::endl;
			return;
		}

		/* always cleanup */
		curl_easy_cleanup(curl);
	}

	write_stream.close();
}

void file_manager::write_blank_png(const std::string& path, unsigned width, unsigned height, const bool overwrite)
{
	make_directory(path);
	std::vector<unsigned char> png;

	//generate some image;
	std::vector<unsigned char> image;
	image.resize(width * height * 4);
	for (unsigned y = 0; y < height; y++)
		for (unsigned x = 0; x < width; x++)
		{
			image[4 * width * y + 4 * x + 0] = 255 * !(x & y);
			image[4 * width * y + 4 * x + 1] = x ^ y;
			image[4 * width * y + 4 * x + 2] = x | y;
			image[4 * width * y + 4 * x + 3] = 255;
		}

	unsigned error = lodepng::encode(png, image, width, height);
	if (!error && !(!overwrite && std::filesystem::exists(path)))
	{
		lodepng::save_file(png, path);
		std::cout << GREEN << "Wrote: " << path << WHITE << std::endl;
	}

	//if there's an error, display it
	if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}

std::vector<std::string> file_manager::get_headers()
{
	if (curl_headers.size() > 0)
	{
		return curl_headers;
	}

	std::vector<std::string> headers;
	char* buf = nullptr;
	char* auth_token = nullptr;
	size_t sz = 0;
	if (_dupenv_s(&buf, &sz, "GITHUB_TOKEN") == 0 && buf != nullptr)
	{
		std::string auth_token = "Authorization: Bearer " + std::string(buf);
		headers.push_back(auth_token);
		free(buf);
	}

	headers.push_back("Accept: application/vnd.github+json");
	headers.push_back("X-GitHub-Api-Version: 2022-11-28");
	headers.push_back("User-Agent: BedrockCLI");

	curl_headers = headers;
	return headers;
}