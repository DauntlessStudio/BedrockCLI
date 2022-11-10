#include "new_item.hpp"

auto bp_default_item = nlohmann::ordered_json::parse(R"({ "format_version": "1.10.0", "minecraft:item": { "description": { "identifier": "namespace:name" }, "components": { "minecraft:max_stack_size": 64 } } })");
auto bp_effect_item = nlohmann::ordered_json::parse(R"({ "format_version": "1.10.0", "minecraft:item": { "description": { "identifier": "namespace:name" }, "components": { "minecraft:use_duration": 30000, "minecraft:max_stack_size": 64, "minecraft:food": { "nutrition": 0, "saturation_modifier": "supernatural", "can_always_eat": true } } } })");
auto rp_default_item = nlohmann::ordered_json::parse(R"({ "format_version": "1.10.0", "minecraft:item": { "description": { "identifier": "namespace:name", "category": "Equipment" }, "components": { "minecraft:icon": "name", "minecraft:render_offsets": "apple" } } })");

void new_item::new_item(int argc, char* argv[])
{
	//parse arguments
	cxxopts::Options options("bed", "Command line tool to help create bedrock addons");
	options.add_options()
		("s,stack", "Maximum stack size", cxxopts::value<int>()->default_value("64"))
		("i,indent", "JSON file indent", cxxopts::value<int>()->default_value("4"))
		("e,edible", "Is edible")
		("n,name", "Item names to add", cxxopts::value<std::vector<std::string>>());

	auto result = options.parse(argc, argv);

	//if arguments are invalid, print help message
	if (!result.count("name"))
	{
		help::output_help(argc, argv);
		return;
	}

	//create items
	for (auto name : result["name"].as<std::vector<std::string>>())
	{
		nlohmann::ordered_json item_bp = result["edible"].as<bool>() ? bp_effect_item : bp_default_item;
		nlohmann::ordered_json item_rp = rp_default_item;

		item_bp["minecraft:item"]["description"]["identifier"] = name;
		item_bp["minecraft:item"]["components"]["minecraft:max_stack_size"] = result["stack"].as<int>();

		item_rp["minecraft:item"]["description"]["identifier"] = name;

		std::string filename = utilities::split(name, ':').back() + ".json";

		file_manager::write_json_to_file(item_bp, file_manager::get_bp_path() + "\\items\\" + filename);
		file_manager::write_json_to_file(item_rp, file_manager::get_rp_path() + "\\items\\" + filename);
	}
}
