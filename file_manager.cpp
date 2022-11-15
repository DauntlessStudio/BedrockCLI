#include "file_manager.hpp"

namespace
{
	std::string project_root;
	std::string behavior_pack;
	std::string resource_pack;
	std::string bp_subname = "behavior_packs";
	std::string rp_subname = "resource_packs";
}

void file_manager::write_json_to_file(const nlohmann::ordered_json& object, const std::string& path, int indent)
{
	make_directory(path);
	std::ofstream output(path);

	try
	{
		output << std::setw(indent) << object << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error writing JSON at: " << path << std::endl << e.what() << std::endl;
	}

	std::cout << "Saved JSON at: " << path << std::endl;
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

void file_manager::make_directory(const std::string& path)
{
	size_t index = path.find_last_of('\\');
	std::string tmp = path.substr(0, index + 1);
	std::filesystem::path dir(tmp);
	if (!std::filesystem::exists(dir))
	{
		std::cout << "Directory Doesn't Exist. Creating..." << std::endl;
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
		std::cout << behavior_pack << std::endl;
		return behavior_pack;
	}

	if (std::filesystem::exists(get_project_root()))
	{
		for (auto const& dir_entry : std::filesystem::directory_iterator(get_project_root() + "behavior_packs"))
		{
			behavior_pack = dir_entry.path().u8string();
			std::cout << behavior_pack << std::endl;
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

std::vector<std::string> file_manager::get_files_in_directory(const std::string& path)
{
	if (!std::filesystem::exists(path))
	{
		return std::vector<std::string>();
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