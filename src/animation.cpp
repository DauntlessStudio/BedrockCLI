#include "include/animation.hpp"

void animation::new_animation(int argc, char* argv[])
{
	//parse arguments
	cxxopts::Options options("anim", "Create animation entry");
	options.add_options()
		("h,help", "View help")
		("l,loop", "Should animation loop")
		("i,indent", "JSON file indent", cxxopts::value<int>()->default_value("4"))
		("c,commands", "Commands to play", cxxopts::value<std::vector<std::string>>()->default_value("/say test"))
		("t,time", "The length of the animation", cxxopts::value<double>()->default_value("1.0"))
		("n,name", "Animation name to add as 'entity.anim'", cxxopts::value<std::string>());

	options.allow_unrecognised_options();
	auto result = options.parse(argc, argv);

	//if arguments are invalid, print help message
	if (!result.count("name") || result.count("help"))
	{
		std::cout << options.help() << std::endl;
		return;
	}

	std::string name = result["name"].as<std::string>();
	std::string filename = utilities::split(name, '.').front();
	std::string anim_name = utilities::split(name, '.').back();

	nlohmann::ordered_json anim_object = { {"format_version", "1.16.0"}, {"animations", {}} };
	std::string anim_directory = file_manager::get_bp_path() + "\\animations\\";
	std::string filepath = file_manager::get_file_in_directory(anim_directory, filename + ".json");
	anim_object = file_manager::read_json_from_file(file_manager::get_file_in_directory(anim_directory, filename + ".json"), anim_object);

	nlohmann::ordered_json anim_entry;
	anim_entry["animation_length"] = result["time"].as<double>();
	anim_entry["loop"] = result.count("loop") > 0;
	anim_entry["timeline"]["0.0"] = result["commands"].as<std::vector<std::string>>();

	anim_object["animations"]["animation." + name] = anim_entry;

	if (filepath.empty())
	{
		filepath = anim_directory + filename + ".json";
	}
	file_manager::write_json_to_file(anim_object, filepath, result["indent"].as<int>());
}

void animation::new_controller(int argc, char* argv[])
{
	//parse arguments
	cxxopts::Options options("ctrl", "Create new animation controller");
	options.add_options()
		("h,help", "View help")
		("i,indent", "JSON file indent", cxxopts::value<int>()->default_value("4"))
		("entry", "The on_entry values", cxxopts::value<std::vector<std::string>>()->implicit_value("/say test"))
		("exit", "The on_exit values", cxxopts::value<std::vector<std::string>>())
		("a,animation", "The animation values", cxxopts::value<std::vector<std::string>>())
		("return", "The query to transition back to default", cxxopts::value<std::string>()->implicit_value("true"))
		("q,query", "The query to transition from default", cxxopts::value<std::string>()->default_value("true"))
		("s,state", "The name of the state to transition to", cxxopts::value<std::string>()->default_value("state_1"))
		("n,name", "Controller name to add as 'entity.anim'", cxxopts::value<std::string>());

	options.allow_unrecognised_options();
	auto result = options.parse(argc, argv);

	//if arguments are invalid, print help message
	if (!result.count("name") || result.count("help"))
	{
		std::cout << options.help() << std::endl;
		return;
	}

	std::string name = result["name"].as<std::string>();
	std::string filename = utilities::split(name, '.').front();
	std::string anim_name = utilities::split(name, '.').back();

	std::string state_name = result["state"].as<std::string>();
	std::string query = result["query"].as<std::string>();

	nlohmann::ordered_json anim_object = { {"format_version", "1.19.0"}, {"animation_controllers", {}} };
	std::string anim_directory = file_manager::get_bp_path() + "\\animation_controllers\\";
	std::string filepath = file_manager::get_file_in_directory(anim_directory, filename + ".json");
	anim_object = file_manager::read_json_from_file(file_manager::get_file_in_directory(anim_directory, filename + ".json"), anim_object);

	nlohmann::ordered_json anim_entry = { {"initial_state", "default"} };
	anim_entry["states"]["default"]["transitions"] = nlohmann::json::array({ nlohmann::json({{state_name, query}}) });
	if (result.count("entry"))
		anim_entry["states"][state_name]["on_entry"] = result["entry"].as<std::vector<std::string>>();
	if (result.count("animation"))
		anim_entry["states"][state_name]["animations"] = result["animation"].as<std::vector<std::string>>();
	if (result.count("return"))
		anim_entry["states"][state_name]["transitions"] = nlohmann::json::array({ nlohmann::json({{"default", result["return"].as<std::string>()}}) });
	if (result.count("exit"))
		anim_entry["states"][state_name]["on_exit"] = result["exit"].as<std::vector<std::string>>();

	anim_object["animation_controllers"]["controller.animation." + name] = anim_entry;

	if (filepath.empty())
	{
		filepath = anim_directory + filename + ".json";
	}
	file_manager::write_json_to_file(anim_object, filepath, result["indent"].as<int>());
}
