#include "include/item.hpp"

auto bp_default_item = nlohmann::ordered_json::parse(R"({ "format_version": "1.10.0", "minecraft:item": { "description": { "identifier": "namespace:name" }, "components": { "minecraft:max_stack_size": 64 } } })");
auto bp_effect_item = nlohmann::ordered_json::parse(R"({ "format_version": "1.10.0", "minecraft:item": { "description": { "identifier": "namespace:name" }, "components": { "minecraft:use_duration": 30000, "minecraft:max_stack_size": 64, "minecraft:food": { "nutrition": 0, "saturation_modifier": "supernatural", "can_always_eat": true } } } })");
auto rp_default_item = nlohmann::ordered_json::parse(R"({ "format_version": "1.10.0", "minecraft:item": { "description": { "identifier": "namespace:name", "category": "Equipment" }, "components": { "minecraft:icon": "name", "minecraft:render_offsets": "apple" } } })");
auto rp_item_texture = nlohmann::ordered_json::parse(R"({ "texture_name": "atlas.items", "texture_data": { } })");
auto rp_player_geo = nlohmann::ordered_json::parse(R"({ "format_version": "1.16.0", "minecraft:geometry": [ { "description": { "identifier": "geometry.player.example_weapon", "texture_width": 64, "texture_height": 64, "visible_bounds_width": 3, "visible_bounds_height": 4.5, "visible_bounds_offset": [0, 1.75, 0] }, "bones": [ { "name": "root", "pivot": [0, 0, 0] }, { "name": "waist", "parent": "root", "pivot": [0, 12, 0] }, { "name": "body", "parent": "waist", "pivot": [0, 24, 0] }, { "name": "head", "parent": "body", "pivot": [0, 24, 0] }, { "name": "hat", "parent": "head", "pivot": [0, 24, 0] }, { "name": "cape", "parent": "body", "pivot": [0, 24, 3] }, { "name": "leftArm", "parent": "body", "pivot": [5, 22, 0] }, { "name": "leftSleeve", "parent": "leftArm", "pivot": [5, 22, 0] }, { "name": "leftItem", "parent": "leftArm", "pivot": [6, 15, 1] }, { "name": "rightArm", "parent": "body", "pivot": [-5, 22, 0] }, { "name": "rightSleeve", "parent": "rightArm", "pivot": [-5, 22, 0] }, { "name": "rightItem", "parent": "rightArm", "pivot": [-6, 15, 1], "locators": { "lead_hold": [-6, 15, 1] } }, { "name": "jacket", "parent": "body", "pivot": [0, 24, 0] }, { "name": "leftLeg", "parent": "root", "pivot": [1.9, 12, 0] }, { "name": "leftPants", "parent": "leftLeg", "pivot": [1.9, 12, 0] }, { "name": "rightLeg", "parent": "root", "pivot": [-1.9, 12, 0] }, { "name": "rightPants", "parent": "rightLeg", "pivot": [-1.9, 12, 0] } ] } ] })");
std::string attachable_default = R"({ "format_version": "1.12.0", "minecraft:attachable": { "description": { "identifier": "namespace:example_weapon", "materials": { "default": "entity_alphatest", "enchanted": "entity_alphatest_glint" }, "textures": { "default": "textures/entity/weapons/allay", "enchanted": "textures/misc/enchanted_item_glint" }, "geometry": { "default": "geometry.player.example_weapon" }, "animations": { "ctrl.example_weapon": "controller.animation.player.mob_weapons.example_weapon", "example_weapon.attack.first_person": "animation.player.example_weapon.attack.first_person", "example_weapon.attack.third_person": "animation.player.example_weapon.attack.third_person", "example_weapon.idle": "animation.player.example_weapon.idle" }, "scripts": { "pre_animation": [ "v.is_first_person = c.is_first_person;" ], "animate": [ "ctrl.example_weapon" ] }, "render_controllers": [ "controller.render.item_default" ] } } })";

void item::new_item(int argc, char* argv[])
{
	//parse arguments
	cxxopts::Options options("nitm", "Create new items");
	options.add_options()
		("h,help", "View help")
		("l,lang", "Lang files to add entries to", cxxopts::value<std::vector<std::string>>()->implicit_value("en_US"))
		("s,stack", "Maximum stack size", cxxopts::value<int>()->default_value("64"))
		("i,indent", "JSON file indent", cxxopts::value<int>()->default_value("4"))
		("e,edible", "Is edible")
		("a,attachable", "Create attachable")
		("n,name", "Item names to add", cxxopts::value<std::vector<std::string>>());

	options.allow_unrecognised_options();
	auto result = options.parse(argc, argv);

	//if arguments are invalid, print help message
	if (!result.count("name") || result.count("help"))
	{
		std::cout << options.help() << std::endl;
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

		std::string filename = utilities::split(name, ':').back();
		std::string namespace_name = utilities::split(name, ':').front();

		file_manager::write_json_to_file(item_bp, file_manager::get_bp_path() + "\\items\\" + filename + ".json", result["indent"].as<int>());
		file_manager::write_json_to_file(item_rp, file_manager::get_rp_path() + "\\items\\" + filename + ".json", result["indent"].as<int>());

		//modify textures/item_texture.json
		nlohmann::ordered_json item_texture = file_manager::read_json_from_file(file_manager::get_rp_path() + "\\textures\\item_texture.json", rp_item_texture);
		item_texture["texture_data"][filename] = { {"textures", "textures/items/" + name} };
		file_manager::write_json_to_file(item_texture, file_manager::get_rp_path() + "\\textures\\item_texture.json", result["indent"].as<int>());

		if (result.count("lang"))
		{
			for (const auto& lang_file : result["lang"].as<std::vector<std::string>>())
			{
				file_manager::add_lang_entry("item." + name + ".name=" + utilities::format_name(filename), lang_file, "item names");
			}
		}

		if (result.count("attachable"))
		{
			//handle attachable
			std::string attach_string = attachable_default;
			utilities::replace_all(attach_string, "example", filename);
			utilities::replace_all(attach_string, "namespace", namespace_name);
			file_manager::write_json_to_file(nlohmann::ordered_json::parse(attach_string), file_manager::get_rp_path() + "\\attachables\\" + filename + ".json", result["indent"].as<int>());

			//player geo
			nlohmann::ordered_json player_geo = rp_player_geo;
			player_geo["minecraft:geometry"][0]["description"]["identifier"] = "geometry.player." + filename;
			file_manager::write_json_to_file(player_geo, file_manager::get_rp_path() + "\\models\\entity\\player\\" + filename + ".geo.json", result["indent"].as<int>());

			//add to player
			entity::add_custom_weapon_entry(name, result["indent"].as<int>());
		}
	}
}
