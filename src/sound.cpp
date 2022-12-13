#include "include\sound.hpp"

auto rp_sound_definition = nlohmann::ordered_json::parse(R"({"format_version": "1.14.0", "sound_definitions": {}})");

void sound::new_sound_definition(int argc, char* argv[])
{
	//parse arguments
	cxxopts::Options options("sdef", "Adds definition to sound_definitions.json");
	options.add_options()
		("h,help", "View help")
		("i,indent", "JSON file indent", cxxopts::value<int>()->default_value("4"))
		("v,volume", "Volume for sounds", cxxopts::value<float>()->default_value("1.0"))
		("p,pitch", "Pitch for sounds", cxxopts::value<float>()->default_value("1.0"))
		("c,category", "Category of definition", cxxopts::value<std::string>()->default_value("neutral"))
		("f,filepath", "Paths to sound files", cxxopts::value <std::vector<std::string>>())
		("d,definition", "Name of vanilla definition to copy into new definition", cxxopts::value <std::string>())
		("n,name", "Name of definitions", cxxopts::value<std::vector<std::string>>());

	options.allow_unrecognised_options();
	auto result = options.parse(argc, argv);

	//if arguments are invalid, print help message
	if (!result.count("name") || result.count("help"))
	{
		std::cout << options.help() << std::endl;
		return;
	}

	std::vector<std::string> sound_name = result["name"].as<std::vector<std::string>>();
	nlohmann::ordered_json sound_definitions = file_manager::read_json_from_file(file_manager::get_rp_path() + "\\sounds\\sound_definitions.json", rp_sound_definition);

	nlohmann::ordered_json definition;
	if (result.count("definition"))
	{
		try
		{
			definition = file_manager::read_json_from_web_page("https://raw.githubusercontent.com/Mojang/bedrock-samples/main/resource_pack/sounds/sound_definitions.json")["sound_definitions"][result["definition"].as<std::string>()];
		}
		catch (const std::exception&)
		{
			std::cerr << "Vanilla sound_definitions.json doesn't contain: " << result["definition"].as<std::string>() << std::endl;
			exit(0);
		}
	}
	else
	{
		definition["category"] = result["category"].as<std::string>();
		if (result.count("filepath"))
		{
			float volume = result["volume"].as<float>();
			float pitch = result["pitch"].as<float>();
			for (const auto& filepath : result["filepath"].as<std::vector<std::string>>())
			{
				std::vector<std::string> files = file_manager::get_files_in_directory(file_manager::get_rp_path() + "\\" + filepath, true);
				for (auto& file : files)
				{
					utilities::replace_all(file, file_manager::get_rp_path() + "\\", "");
					utilities::replace_all(file, "\\", "/");
					utilities::replace_all(file, ".ogg", "");
					definition["sounds"].push_back({ {"name", file}, {"pitch", pitch}, {"volume", volume} });;
				}
			}
		}
	}

	for (const auto& sound : sound_name)
	{
		sound_definitions["sound_definitions"][sound] = definition;
	}

	file_manager::write_json_to_file(sound_definitions, file_manager::get_rp_path() + "\\sounds\\sound_definitions.json", result["indent"].as<int>());
}
