#include "file_manager.hpp"

namespace
{
	std::string project_root;
	std::string behavior_pack;
	std::string resource_pack;
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
