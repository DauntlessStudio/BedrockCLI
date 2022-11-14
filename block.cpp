#include "block.hpp"

auto bp_default_block = nlohmann::ordered_json::parse(R"({ "format_version": "1.12.0", "minecraft:block": { "description": { "identifier": "namespace:name", "register_to_creative_menu": true }, "components": { "minecraft:block_light_emission": 0, "minecraft:destroy_time": 2.0, "minecraft:explosion_resistance": 1, "minecraft:friction": 0.6, "minecraft:loot": "loot_tables/blocks/empty.json", "minecraft:map_color": "#B0B0B0" } } })");
auto rp_terrain_tex = nlohmann::ordered_json::parse(R"({ "num_mip_levels" : 4, "padding" : 8, "resource_pack_name" : "vanilla", "texture_data" : { }, "texture_name" : "atlas.terrain" })");

void block::new_block(int argc, char* argv[])
{
	//parse arguments
	cxxopts::Options options("bed", "Command line tool to help create bedrock addons");
	options.add_options()
		("l,loot", "Add loot table")
		("i,indent", "JSON file indent", cxxopts::value<int>()->default_value("4"))
		("e,emissive", "Block emits light", cxxopts::value<double>()->default_value("0.0"))
		("n,name", "Block names to add", cxxopts::value<std::vector<std::string>>());

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
		nlohmann::ordered_json block_bp = bp_default_block;

		std::string filename = utilities::split(name, ':').back();

		if (result.count("loot"))
		{
			block_bp["minecraft:block"]["components"]["minecraft:loot"] = "loot_tables/blocks/" + filename + ".json";
			nlohmann::ordered_json loot_table;
			loot_table["pools"][0]["rolls"] = 1;
			loot_table["pools"][0]["entries"][0] = { {"type", "item"}, {"name", name}, {"weight", 1} };
			file_manager::write_json_to_file(loot_table, file_manager::get_bp_path() + "\\loot_tables\\blocks\\" + filename + ".json");
		}
		block_bp["minecraft:block"]["components"]["minecraft:block_light_emission"] = std::clamp(result["emissive"].as<double>(), 0.0, 1.0);

		file_manager::write_json_to_file(block_bp, file_manager::get_bp_path() + "\\blocks\\" + filename + ".json");

		//modify textures/block_texture.json
		nlohmann::ordered_json item_texture = file_manager::read_json_from_file(file_manager::get_rp_path() + "\\textures\\terrain_texture.json", rp_terrain_tex);
		item_texture["texture_data"][filename] = { {"textures", "textures/blocks/" + name} };
		file_manager::write_json_to_file(item_texture, file_manager::get_rp_path() + "\\textures\\terrain_texture.json");
	}
}
