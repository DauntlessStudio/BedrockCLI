#include "entity.hpp"

auto bp_dummy_entity = nlohmann::ordered_json::parse(R"({ "format_version": "1.18.0", "minecraft:entity": { "description": { "identifier": "namespace:name", "is_spawnable": true, "is_summonable": true, "is_experimental": false }, "component_groups": { "instant_despawn": { "minecraft:instant_despawn": {} } }, "components": { "minecraft:type_family": { "family": [ "namespace", "name" ] }, "minecraft:collision_box": { "height": 0, "width": 0 }, "minecraft:physics": { "has_collision": false, "has_gravity": false }, "minecraft:damage_sensor": { "triggers": [ { "on_damage": { "filters": { "test": "has_damage", "value": "void" }, "event": "despawn" }, "deals_damage": false }, { "cause": "all", "deals_damage": false } ] } }, "events": { "despawn": { "add": { "component_groups": [ "instant_despawn" ] } } } } })");
auto bp_passive_entity = nlohmann::ordered_json::parse(R"({ "format_version": "1.18.2", "minecraft:entity": { "description": { "identifier": "namespace:entity", "is_spawnable": true, "is_summonable": true, "is_experimental": false }, "component_groups": { "despawn": { "minecraft:instant_despawn": {} } }, "components": { "minecraft:is_hidden_when_invisible": {}, "minecraft:type_family": { "family": [ "mob", "namespace", "name" ] }, "minecraft:breathable": { "total_supply": 15, "suffocate_time": 0 }, "minecraft:nameable": {}, "minecraft:pushable": { "is_pushable": true, "is_pushable_by_piston": true }, "minecraft:conditional_bandwidth_optimization": {}, "minecraft:movement": { "value": 0.25 }, "minecraft:navigation.walk": { "avoid_damage_blocks": true }, "minecraft:movement.basic": {}, "minecraft:jump.static": {}, "minecraft:can_climb": {}, "minecraft:collision_box": { "width": 0.6, "height": 1.8 }, "minecraft:physics": {}, "minecraft:despawn": { "despawn_from_distance": {} }, "minecraft:behavior.float": { "priority": 2 }, "minecraft:behavior.panic": { "priority": 3, "speed_multiplier": 1.25 }, "minecraft:behavior.random_stroll": { "priority": 7 }, "minecraft:behavior.look_at_player": { "priority": 8, "look_distance": 6.0 }, "minecraft:behavior.random_look_around": { "priority": 9 } }, "events": { "despawn": { "add": { "component_groups": [ "despawn" ] } } } } })");
auto bp_hostile_entity = nlohmann::ordered_json::parse(R"({ "format_version": "1.18.2", "minecraft:entity": { "description": { "identifier": "namespace:entity", "is_spawnable": true, "is_summonable": true, "is_experimental": false }, "component_groups": { "despawn": { "minecraft:instant_despawn": {} } }, "components": { "minecraft:is_hidden_when_invisible": {}, "minecraft:type_family": { "family": [ "namespace", "entity", "mob" ] }, "minecraft:breathable": { "total_supply": 15, "suffocate_time": 0 }, "minecraft:nameable": {}, "minecraft:pushable": { "is_pushable": true, "is_pushable_by_piston": true }, "minecraft:conditional_bandwidth_optimization": {}, "minecraft:movement": { "value": 0.25 }, "minecraft:navigation.walk": { "avoid_damage_blocks": true }, "minecraft:movement.basic": {}, "minecraft:jump.static": {}, "minecraft:can_climb": {}, "minecraft:collision_box": { "width": 0.6, "height": 1.8 }, "minecraft:physics": {}, "minecraft:despawn": { "despawn_from_distance": {} }, "minecraft:attack": { "damage": 3 }, "minecraft:behavior.nearest_prioritized_attackable_target": { "priority": 0, "entity_types":[ { "filters": { "test": "is_family", "value": "player", "subject": "other" } } ] }, "minecraft:behavior.melee_attack": { "priority": 1, "speed_multiplier": 1.2, "reach_multiplier": 2 }, "minecraft:behavior.float": { "priority": 2 }, "minecraft:behavior.panic": { "priority": 3, "speed_multiplier": 1.25 }, "minecraft:behavior.random_stroll": { "priority": 7 }, "minecraft:behavior.look_at_player": { "priority": 8, "look_distance": 6.0 }, "minecraft:behavior.random_look_around": { "priority": 9 } }, "events": { "despawn": { "add": { "component_groups": [ "despawn" ] } } } } })");
auto rp_entity_default = nlohmann::ordered_json::parse(R"({ "format_version": "1.18.0", "minecraft:client_entity": { "description": { "identifier": "namespace:name", "min_engine_version": "1.10.0", "materials": { "default": "entity_alphatest" }, "textures": { "default": "textures/entity/name/default" }, "geometry": { "default": "geometry.name" }, "render_controllers": [ "controller.render.default" ] } } })");
auto rp_geo_default = nlohmann::ordered_json::parse(R"({ "format_version": "1.18.0", "minecraft:geometry": [ { "description": { "identifier": "geometry.name", "texture_width": 16, "texture_height": 16, "visible_bounds_width": 2, "visible_bounds_height": 3, "visible_bounds_offset": [0, 0.5, 0] }, "bones": [ { "name": "body", "pivot": [0, 0, 0], "cubes": [ {"origin": [-8, 0, -8], "size": [16, 16, 16], "uv": [0, 0]} ] } ] } ] })");

void entity::new_entity(int argc, char* argv[])
{
	//parse arguments
	cxxopts::Options options("bed", "Command line tool to help create bedrock addons");
	options.add_options()
		("e,enemy", "Hostile enemy")
		("p,passive", "Passive mob")
		("m,model", "Create new model with entity")
		("i,indent", "JSON file indent", cxxopts::value<int>()->default_value("4"))
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
		nlohmann::ordered_json bp_entity = result.count("enemy") ? bp_hostile_entity : result.count("passive") ? bp_passive_entity : bp_dummy_entity;
		nlohmann::ordered_json rp_entity = rp_entity_default;

		std::string filename = utilities::split(name, ':').back();
		std::string entity_namespace = utilities::split(name, ':').front();

		bp_entity["minecraft:entity"]["description"]["identifier"] = name;
		bp_entity["minecraft:entity"]["components"]["minecraft:type_family"]["family"] = {entity_namespace, filename};

		rp_entity["minecraft:client_entity"]["description"]["identifier"] = name;
		rp_entity["minecraft:client_entity"]["description"]["geometry"]["default"] = "geometry." + filename;
		rp_entity["minecraft:client_entity"]["description"]["textures"]["default"] = "textures/entity/" + filename + "/default";

		file_manager::write_json_to_file(bp_entity, file_manager::get_bp_path() + "\\entities\\" + filename + ".json", result["indent"].as<int>());
		file_manager::write_json_to_file(rp_entity, file_manager::get_rp_path() + "\\entity\\" + filename + ".json", result["indent"].as<int>());

		if (result.count("model"))
		{
			nlohmann::ordered_json rp_geo = rp_geo_default;
			rp_geo["minecraft:geometry"][0]["description"]["identifier"] = "geometry." + filename;

			file_manager::write_json_to_file(rp_geo, file_manager::get_rp_path() + "\\models\\entity\\" + filename + ".json", result["indent"].as<int>());
		}
	}
}
