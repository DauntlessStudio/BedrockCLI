#include "include\package_manager.hpp"

void package_manager::import_package(int argc, char* argv[])
{
	//parse arguments
	cxxopts::Options options("pkg", "Import packages");
	options.add_options()
		("h,help", "View help")
		("l,list", "Lists available packages")
		("import", "Import package using either name or index", cxxopts::value<std::string>())
		("i,indent", "JSON file indent", cxxopts::value<int>()->default_value("4"));

	options.allow_unrecognised_options();
	auto result = options.parse(argc, argv);

	//if arguments are invalid, print help message
	if (!(result.count("list") || result.count("import")) || result.count("help"))
	{
		std::cout << options.help() << std::endl;
		return;
	}

	if (result.count("list"))
	{
		nlohmann::ordered_json root_list = file_manager::read_json_from_web_page("https://api.github.com/repos/Dotzip-Developments/DotzipPackages/contents/", file_manager::get_headers());
		for (const auto& item : root_list.items())
		{
			std::cout << "[" << item.key() << "] " << item.value()["name"] << std::endl;
		}
	}

	if (result.count("import"))
	{
		std::string import_id = result["import"].as<std::string>();
		nlohmann::ordered_json root_list = file_manager::read_json_from_web_page("https://api.github.com/repos/Dotzip-Developments/DotzipPackages/contents/", file_manager::get_headers());
		try
		{
			int import_index = std::stoi(import_id);
			std::cout << "Importing: " << root_list[import_index]["name"] << std::endl;
			recursive_download(root_list[import_index]["url"]);
		}
		catch (const std::exception&)
		{
			for (const auto& item : root_list.items())
			{
				if (item.value()["name"] == import_id)
				{
					std::cout << "Importing: " << item.value()["name"] << std::endl;
					recursive_download(item.value()["url"]);
				}
			}
		}
	}
}

void package_manager::recursive_download(const std::string& path)
{
	nlohmann::ordered_json root_list = file_manager::read_json_from_web_page(path, file_manager::get_headers());

	for (const auto& item : root_list.items())
	{
		if (item.value()["type"] == "dir")
		{
			std::string val = item.value()["url"];
			recursive_download(val);
			continue;
		}

		std::string path = std::string(item.value()["path"]);
		utilities::replace_all(path, "/", "\\");
		path = std::regex_replace(path, std::regex(".+BP"), file_manager::get_bp_path());
		path = std::regex_replace(path, std::regex(".+RP"), file_manager::get_rp_path());

		// handle json files
		if (path.find(".json") != std::string::npos)
		{
			nlohmann::ordered_json web_json = file_manager::read_json_from_web_page(item.value()["download_url"], file_manager::get_headers());
			nlohmann::ordered_json file_json = file_manager::read_json_from_file(path, web_json);
			if (web_json != file_json)
			{
				web_json.merge_patch(file_json);
			}
			
			file_manager::write_json_to_file(web_json, path);
			continue;
		}

		// handle lang files
		if (path.find(".lang") != std::string::npos)
		{
			std::string entry = file_manager::read_string_from_web_page(item.value()["download_url"], file_manager::get_headers());
			entry = std::regex_replace(entry, std::regex("([^a-zA-Z0-9\\.:_= '\", ])"), "");

			std::string name = item.value()["name"];
			utilities::replace_all(name, ".lang", "");
			file_manager::add_lang_entry(entry, name, "entity names");
			continue;
		}

		// handle other files
		file_manager::write_file_from_web_page(item.value()["download_url"], path, file_manager::get_headers());
	}
}
