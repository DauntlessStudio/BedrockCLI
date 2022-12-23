#include "include/entity.hpp"

auto bp_dummy_entity = nlohmann::ordered_json::parse(R"({ "format_version": "1.18.0", "minecraft:entity": { "description": { "identifier": "namespace:name", "is_spawnable": false, "is_summonable": true, "is_experimental": false }, "component_groups": { "instant_despawn": { "minecraft:instant_despawn": {} } }, "components": { "minecraft:type_family": { "family": [ "namespace", "name" ] }, "minecraft:collision_box": { "height": 0, "width": 0 }, "minecraft:physics": { "has_collision": false, "has_gravity": false }, "minecraft:damage_sensor": { "triggers": [ { "on_damage": { "filters": { "test": "has_damage", "value": "void" }, "event": "despawn" }, "deals_damage": false }, { "cause": "all", "deals_damage": false } ] } }, "events": { "despawn": { "add": { "component_groups": [ "instant_despawn" ] } } } } })");
auto bp_passive_entity = nlohmann::ordered_json::parse(R"({ "format_version": "1.18.2", "minecraft:entity": { "description": { "identifier": "namespace:entity", "is_spawnable": true, "is_summonable": true, "is_experimental": false }, "component_groups": { "despawn": { "minecraft:instant_despawn": {} } }, "components": { "minecraft:is_hidden_when_invisible": {}, "minecraft:type_family": { "family": [ "mob", "namespace", "name" ] }, "minecraft:breathable": { "total_supply": 15, "suffocate_time": 0 }, "minecraft:nameable": {}, "minecraft:pushable": { "is_pushable": true, "is_pushable_by_piston": true }, "minecraft:conditional_bandwidth_optimization": {}, "minecraft:movement": { "value": 0.25 }, "minecraft:navigation.walk": { "avoid_damage_blocks": true }, "minecraft:movement.basic": {}, "minecraft:jump.static": {}, "minecraft:can_climb": {}, "minecraft:collision_box": { "width": 0.6, "height": 1.8 }, "minecraft:physics": {}, "minecraft:despawn": { "despawn_from_distance": {} }, "minecraft:behavior.float": { "priority": 2 }, "minecraft:behavior.panic": { "priority": 3, "speed_multiplier": 1.25 }, "minecraft:behavior.random_stroll": { "priority": 7 }, "minecraft:behavior.look_at_player": { "priority": 8, "look_distance": 6.0 }, "minecraft:behavior.random_look_around": { "priority": 9 } }, "events": { "despawn": { "add": { "component_groups": [ "despawn" ] } } } } })");
auto bp_hostile_entity = nlohmann::ordered_json::parse(R"({ "format_version": "1.18.2", "minecraft:entity": { "description": { "identifier": "namespace:entity", "is_spawnable": true, "is_summonable": true, "is_experimental": false }, "component_groups": { "despawn": { "minecraft:instant_despawn": {} } }, "components": { "minecraft:is_hidden_when_invisible": {}, "minecraft:type_family": { "family": [ "namespace", "entity", "mob" ] }, "minecraft:breathable": { "total_supply": 15, "suffocate_time": 0 }, "minecraft:nameable": {}, "minecraft:pushable": { "is_pushable": true, "is_pushable_by_piston": true }, "minecraft:conditional_bandwidth_optimization": {}, "minecraft:movement": { "value": 0.25 }, "minecraft:navigation.walk": { "avoid_damage_blocks": true }, "minecraft:movement.basic": {}, "minecraft:jump.static": {}, "minecraft:can_climb": {}, "minecraft:collision_box": { "width": 0.6, "height": 1.8 }, "minecraft:physics": {}, "minecraft:despawn": { "despawn_from_distance": {} }, "minecraft:attack": { "damage": 3 }, "minecraft:behavior.nearest_prioritized_attackable_target": { "priority": 0, "entity_types":[ { "filters": { "test": "is_family", "value": "player", "subject": "other" } } ] }, "minecraft:behavior.melee_attack": { "priority": 1, "speed_multiplier": 1.2, "reach_multiplier": 2 }, "minecraft:behavior.float": { "priority": 2 }, "minecraft:behavior.panic": { "priority": 3, "speed_multiplier": 1.25 }, "minecraft:behavior.random_stroll": { "priority": 7 }, "minecraft:behavior.look_at_player": { "priority": 8, "look_distance": 6.0 }, "minecraft:behavior.random_look_around": { "priority": 9 } }, "events": { "despawn": { "add": { "component_groups": [ "despawn" ] } } } } })");
auto rp_entity_default = nlohmann::ordered_json::parse(R"({ "format_version": "1.18.0", "minecraft:client_entity": { "description": { "identifier": "namespace:name", "min_engine_version": "1.10.0", "materials": { "default": "entity_alphatest" }, "textures": { "default": "textures/entity/name/default" }, "geometry": { "default": "geometry.name" }, "render_controllers": [ "controller.render.default" ] } } })");
auto rp_geo_default = nlohmann::ordered_json::parse(R"({ "format_version": "1.18.0", "minecraft:geometry": [ { "description": { "identifier": "geometry.name", "texture_width": 16, "texture_height": 16, "visible_bounds_width": 2, "visible_bounds_height": 3, "visible_bounds_offset": [0, 0.5, 0] }, "bones": [ { "name": "body", "pivot": [0, 0, 0], "cubes": [ {"origin": [-8, 0, -8], "size": [16, 16, 16], "uv": [0, 0]} ] } ] } ] })");
auto rp_player_ac_default = nlohmann::ordered_json::parse(R"({ "format_version": "1.19.10", "animation_controllers": {} })");
auto rp_player_anim = nlohmann::ordered_json::parse(R"({ "format_version": "1.8.0", "animations": {} })");

void entity::new_entity(int argc, char* argv[])
{
	//parse arguments
	cxxopts::Options options("nent", "Create new entities");
	options.add_options()
		("h,help", "View help")
		("l,lang", "Lang files to add entries to", cxxopts::value<std::vector<std::string>>()->implicit_value("en_US"))
		("e,enemy", "Create hostile enemy")
		("p,passive", "Create passive mob")
		("m,model", "Create new model with entity")
		("r,rp", "Create rp entry for entity")
		("i,indent", "JSON file indent", cxxopts::value<int>()->default_value("4"))
		("n,name", "Entity names to add", cxxopts::value<std::vector<std::string>>());

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
		nlohmann::ordered_json bp_entity = result.count("enemy") ? bp_hostile_entity : result.count("passive") ? bp_passive_entity : bp_dummy_entity;

		utilities::name ent_name = utilities::name(name);

		bp_entity["minecraft:entity"]["description"]["identifier"] = ent_name.internal_name;
		bp_entity["minecraft:entity"]["components"]["minecraft:type_family"]["family"] = { ent_name.space, ent_name.shortname};
		file_manager::write_json_to_file(bp_entity, file_manager::get_bp_path() + "\\entities\\" + ent_name.filename + ".json", result["indent"].as<int>());

		if (result.count("rp"))
		{
			nlohmann::ordered_json rp_entity = rp_entity_default;
			rp_entity["minecraft:client_entity"]["description"]["identifier"] = ent_name.internal_name;
			rp_entity["minecraft:client_entity"]["description"]["geometry"]["default"] = "geometry." + ent_name.shortname;
			rp_entity["minecraft:client_entity"]["description"]["textures"]["default"] = "textures/entity/" + ent_name.shortname + "/default";

			file_manager::write_json_to_file(rp_entity, file_manager::get_rp_path() + "\\entity\\" + ent_name.filename + ".entity.json", result["indent"].as<int>());
		}

		if (result.count("model"))
		{
			nlohmann::ordered_json rp_geo = rp_geo_default;
			rp_geo["minecraft:geometry"][0]["description"]["identifier"] = "geometry." + ent_name.shortname;

			file_manager::write_json_to_file(rp_geo, file_manager::get_rp_path() + "\\models\\entity\\" + ent_name.filename + ".geo.json", result["indent"].as<int>());
		}

		if (result.count("lang"))
		{
			for (const auto& lang_file : result["lang"].as<std::vector<std::string>>())
			{
				file_manager::add_lang_entry("entity." + ent_name.internal_name + ".name=" + utilities::format_name(ent_name.shortname), lang_file, "entity names");
				file_manager::add_lang_entry("item.spawn_egg." + ent_name.internal_name + ".name=Spawn " + utilities::format_name(ent_name.shortname), lang_file, "spawn eggs");
			}
		}
	}
}

void entity::player_entity(int argc, char* argv[])
{
	//parse arguments
	cxxopts::Options options("pent", "Add player entity");
	options.add_options()
		("h,help", "View help")
		("b,bp", "Create bp entry for entity")
		("r,rp", "Create rp entry for entity")
		("g,geo", "Initialize for 3D geos")
		("i,indent", "JSON file indent", cxxopts::value<int>()->default_value("4"));

	options.allow_unrecognised_options();
	auto result = options.parse(argc, argv);

	//if arguments are invalid, print help message
	if (!(result.count("bp") || result.count("rp")) || result.count("help"))
	{
		std::cout << options.help() << std::endl;
		return;
	}

	//create player entity

	if (result.count("rp"))
	{
		nlohmann::ordered_json rp_entity = file_manager::read_json_from_web_page("https://raw.githubusercontent.com/Mojang/bedrock-samples/main/resource_pack/entity/player.entity.json");
		rp_entity = file_manager::read_json_from_file(file_manager::get_rp_path() + "\\entity\\player.entity.json", rp_entity);

		if (result.count("geo"))
		{
			rp_entity["minecraft:client_entity"]["description"]["scripts"]["pre_animation"].push_back("v.has_custom_weapon = 0;");
			rp_entity["minecraft:client_entity"]["description"]["scripts"]["pre_animation"].push_back("v.dampen_left_arm_swing  = 0;");
			rp_entity["minecraft:client_entity"]["description"]["scripts"]["pre_animation"].push_back("v.dampen_right_arm_swing = 0;");
			rp_entity["minecraft:client_entity"]["description"]["scripts"]["pre_animation"].push_back("v.disable_arm_swing = 0;");
			rp_entity["minecraft:client_entity"]["description"]["scripts"]["pre_animation"].push_back("v.disable_leg_swing = q.is_riding;");
			rp_entity["minecraft:client_entity"]["description"]["scripts"]["pre_animation"].push_back("v.aim_left_arm = 0;");
			rp_entity["minecraft:client_entity"]["description"]["scripts"]["pre_animation"].push_back("v.aim_right_arm = 0;");
			rp_entity["minecraft:client_entity"]["description"]["scripts"]["animate"].push_back("ctrl.custom_weapon.select");
			rp_entity["minecraft:client_entity"]["description"]["scripts"]["animate"].push_back({ {"custom_weapon.first_person.base", "v.is_first_person && v.has_custom_weapon"} });
			rp_entity["minecraft:client_entity"]["description"]["scripts"]["animate"].push_back({ {"custom_weapon.third_person.right", "!v.is_first_person && v.has_custom_weapon && v.aim_left_arm"} });
			rp_entity["minecraft:client_entity"]["description"]["scripts"]["animate"].push_back({ {"custom_weapon.third_person.left", "!v.is_first_person && v.has_custom_weapon && v.aim_right_arm"} });
			rp_entity["minecraft:client_entity"]["description"]["animations"]["ctrl.custom_weapon.select"] = "controller.animation.player.custom_weapon.select";
			rp_entity["minecraft:client_entity"]["description"]["animations"]["custom_weapon.first_person.base"] = "animation.player.custom_weapon.base_first_person_pose";
			rp_entity["minecraft:client_entity"]["description"]["animations"]["custom_weapon.third_person.right"] = "animation.player.custom_weapon.third_person_aim_arm.right";
			rp_entity["minecraft:client_entity"]["description"]["animations"]["custom_weapon.third_person.left"] = "animation.player.custom_weapon.third_person_aim_arm.left";

			nlohmann::ordered_json rp_anim_controller = file_manager::read_json_from_file(file_manager::get_rp_path() + "\\animation_controllers\\player.ac.json", rp_player_ac_default);
			rp_anim_controller["animation_controllers"]["controller.animation.player.custom_weapon.select"]["initial_state"] = "no_weapon";
			rp_anim_controller["animation_controllers"]["controller.animation.player.root"] = nlohmann::ordered_json::parse(R"({ "initial_state" : "first_person", "states" : { "first_person" : { "animations" : [ "first_person_swap_item", { "first_person_attack_controller" : "v.attack_time > 0.0f && q.get_equipped_item_name != 'map' && !v.has_custom_weapon" }, { "first_person_base_pose": "!v.has_custom_weapon" }, { "first_person_empty_hand" : "q.get_equipped_item_name(0, 1) != 'map' && !v.has_custom_weapon" }, { "first_person_walk" : "v.bob_animation" }, { "first_person_map_controller" : "(q.get_equipped_item_name(0, 1) == 'map' || q.get_equipped_item_name('off_hand') == 'map') " }, { "first_person_crossbow_equipped": "q.get_equipped_item_name == 'crossbow' && (v.item_use_normalized > 0 && v.item_use_normalized < 1.0) " }, { "first_person_breathing_bob": "v.attack_time <= 0.0 || v.has_custom_weapon" } ], "transitions" : [ { "paperdoll" : "v.is_paperdoll" }, { "map_player" : "v.map_face_icon" }, { "third_person" : "!v.is_first_person && !q.is_saddled" } ] }, "map_player" : { "transitions" : [ { "paperdoll" : "v.is_paperdoll" }, { "first_person" : "v.is_first_person" }, { "third_person" : "!v.map_face_icon && !v.is_first_person" } ] }, "paperdoll" : { "animations" : [ "humanoid_base_pose", "look_at_target_ui", "move.arms", "move.legs", "cape" ], "transitions" : [ { "first_person" : "!v.is_paperdoll && v.is_first_person" }, { "map_player" : "v.map_face_icon" }, { "third_person" : "!v.is_paperdoll && !v.is_first_person" } ] }, "third_person" : { "animations" : [ "humanoid_base_pose", { "look_at_target" : "!q.is_sleeping && !q.is_emoting" }, { "move.arms": "!v.disable_arm_swing" }, "move.legs", "cape", { "riding.arms" : "q.is_riding && !v.disable_leg_swing" }, { "riding.legs" : "q.is_riding && !v.disable_leg_swing" }, { "holding": "!v.has_custom_weapon" }, { "brandish_spear" : "v.is_brandishing_spear" }, { "holding_spyglass": "v.is_holding_spyglass" }, { "charging" : "q.is_charging" }, { "sneaking" : "q.is_sneaking && !q.is_sleeping" }, { "bob": "!v.is_holding_spyglass" }, { "damage_nearby_mobs" : "v.damage_nearby_mobs" }, { "swimming" : "v.swim_amount > 0.0" }, { "swimming.legs" : "v.swim_amount > 0.0" }, { "use_item_progress" : "( v.use_item_interval_progress > 0.0 ) || ( v.use_item_startup_progress > 0.0 ) && !v.is_brandishing_spear && !v.is_holding_spyglass && !v.has_custom_weapon" }, { "sleeping" : "q.is_sleeping && q.is_alive" }, { "attack.positions" : "v.attack_time >= 0.0" }, { "attack.rotations" : "v.attack_time >= 0.0 && !v.has_custom_weapon" }, { "shield_block_main_hand" : "q.blocking && q.get_equipped_item_name('off_hand') != 'shield' && q.get_equipped_item_name == 'shield'" }, { "shield_block_off_hand" : "q.blocking && q.get_equipped_item_name('off_hand') == 'shield'" }, { "crossbow_controller" : "q.get_equipped_item_name == 'crossbow'" }, { "third_person_bow_equipped" : "q.get_equipped_item_name == 'bow' && (v.item_use_normalized > 0 && v.item_use_normalized < 1.0) " } ], "transitions" : [ { "paperdoll" : "v.is_paperdoll" }, { "first_person" : "v.is_first_person" }, { "map_player" : "v.map_face_icon" } ] }, "first_person_transition" : { "transitions" : [ { "first_person" : "1" } ], "blend_via_shortest_path": true }, "third_person_transition" : { "transitions" : [ { "third_person" : "1" } ], "blend_via_shortest_path": true } } })");
			file_manager::write_json_to_file(rp_anim_controller, file_manager::get_rp_path() + "\\animation_controllers\\player.ac.json", result["indent"].as<int>());

			nlohmann::ordered_json rp_anim = file_manager::read_json_from_file(file_manager::get_rp_path() + "\\animations\\player.anim.json", rp_player_anim);
			rp_anim["animations"]["animation.player.move.arms"] = nlohmann::ordered_json::parse(R"({ "loop": true, "bones": { "leftArm": { "rotation": [ "variable.tcos0/(5 * v.dampen_left_arm_swing + 1) ", 0, 0 ] }, "rightArm": { "rotation": [ "-variable.tcos0/(5 * v.dampen_right_arm_swing + 1) ", 0, 0 ] } } })");
			rp_anim["animations"]["animation.player.custom_weapon.base_first_person_pose"] = nlohmann::ordered_json::parse(R"({ "loop": true, "bones": { "root": { "rotation": [0, 180, 0], "position": [0, 0, 0] }, "leftArm": { "position": [10, 0, -5] }, "rightArm": { "position": [-10, 0, -5] } } })");
			rp_anim["animations"]["animation.player.custom_weapon.third_person_aim_arm.right"] = nlohmann::ordered_json::parse(R"({ "loop": true, "bones": { "rightArm": { "rotation": ["q.target_x_rotation - (q.is_sneaking * 15) ", "q.target_y_rotation", 0] } } })");
			rp_anim["animations"]["animation.player.custom_weapon.third_person_aim_arm.left"] = nlohmann::ordered_json::parse(R"({ "loop": true, "bones": { "leftArm": { "rotation": ["q.target_x_rotation - (q.is_sneaking * 15) ", "q.target_y_rotation", 0] } } })");
			file_manager::write_json_to_file(rp_anim, file_manager::get_rp_path() + "\\animations\\player.anim.json", result["indent"].as<int>());
		}

		file_manager::write_json_to_file(rp_entity, file_manager::get_rp_path() + "\\entity\\player.entity.json", result["indent"].as<int>());
	}

	if (result.count("bp"))
	{
		nlohmann::ordered_json bp_entity = file_manager::read_json_from_web_page("https://raw.githubusercontent.com/Mojang/bedrock-samples/main/behavior_pack/entities/player.json");
		bp_entity = file_manager::read_json_from_file(file_manager::get_bp_path() + "\\entities\\player.json", bp_entity);

		file_manager::write_json_to_file(bp_entity, file_manager::get_bp_path() + "\\entities\\player.json", result["indent"].as<int>());
	}
}

void entity::component_group(int argc, char* argv[])
{
	//parse arguments
	cxxopts::Options options("cogr", "Attaches or removes component groups for the entities");
	options.add_options()
		("h,help", "View help")
		("g,group", "The component group to add/remove, either a name or JSON string", cxxopts::value<std::string>())
		("i,indent", "JSON file indent", cxxopts::value<int>()->default_value("4"))
		("f,family", "Family types to modify", cxxopts::value<std::vector<std::string>>()->default_value(""))
		("d,directory", "Subdirectory to modify", cxxopts::value <std::string>()->default_value(""))
		("r,remove", "Remove group")
		("n,name", "Filenames of entities to modify, i.e. player.json", cxxopts::value<std::vector<std::string>>()->default_value(""));

	options.allow_unrecognised_options();
	auto result = options.parse(argc, argv);

	//if arguments are invalid, print help message
	if (!result.count("group") || result.count("help"))
	{
		std::cout << options.help() << std::endl;
		return;
	}

	nlohmann::ordered_json group;
	try
	{
		group = nlohmann::ordered_json::parse(result["group"].as<std::string>());
	}
	catch (const std::exception&)
	{
		group[result["group"].as<std::string>()] = nlohmann::json::object();
	}

	std::vector<entity> entities = get_valid_entities(result["directory"].as<std::string>(), result["family"].as<std::vector<std::string>>(), result["name"].as<std::vector<std::string>>());

	//add component group to entity list
	if (!result.count("remove"))
	{
		for (auto& ent : entities)
		{
			ent.add_component_group(group);
			ent.add_event(group.items().begin().key(), true);
			ent.write_entity(result["indent"].as<int>());
		}
		return;
	}

	//remove component group from entity list
	for (auto& ent : entities)
	{
		ent.remove_component_group(result["group"].as<std::string>());
		ent.remove_event(result["group"].as<std::string>());
		ent.write_entity(result["indent"].as<int>());
	}
}

void entity::component(int argc, char* argv[])
{
	//parse arguments
	cxxopts::Options options("comp", "Attaches or removes components for the entites");
	options.add_options()
		("h,help", "View help")
		("c,component", "Component to add in JSON format, or the name of the component to remove", cxxopts::value<std::string>())
		("i,indent", "JSON file indent", cxxopts::value<int>()->default_value("4"))
		("f,family", "Family types to modify", cxxopts::value<std::vector<std::string>>()->default_value(""))
		("d,directory", "Subdirectory to modify", cxxopts::value <std::string>()->default_value(""))
		("r,remove", "Remove component")
		("n,name", "Filenames of entities to modify", cxxopts::value<std::vector<std::string>>()->default_value(""));

	options.allow_unrecognised_options();
	auto result = options.parse(argc, argv);

	//if arguments are invalid, print help message
	if (!result.count("component") || result.count("help"))
	{
		std::cout << options.help() << std::endl;
		return;
	}

	nlohmann::ordered_json component;
	try
	{
		component = nlohmann::ordered_json::parse(result["component"].as<std::string>());
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}

	std::vector<entity> entities = get_valid_entities(result["directory"].as<std::string>(), result["family"].as<std::vector<std::string>>(), result["name"].as<std::vector<std::string>>());

	//add component to entity list
	if (!result.count("remove"))
	{
		for (auto& ent : entities)
		{
			ent.add_component(component);
			ent.write_entity(result["indent"].as<int>());
		}
		return;
	}

	//remove component group from entity list
	for (auto& ent : entities)
	{
		ent.remove_component_group(result["component"].as<std::string>());
		ent.write_entity(result["indent"].as<int>());
	}
}

void entity::animation(int argc, char* argv[])
{
	//parse arguments
	cxxopts::Options options("eanim", "Attaches or removes animations for the entites");
	options.add_options()
		("h,help", "View help")
		("a,animation", "Animation names to add/remove as 'entity.animation'", cxxopts::value<std::vector<std::string>>())
		("s,script", "Add animation to scripts")
		("c,controller", "Is animation a controller")
		("i,indent", "JSON file indent", cxxopts::value<int>()->default_value("4"))
		("f,family", "Family types to modify", cxxopts::value<std::vector<std::string>>()->default_value(""))
		("d,directory", "Subdirectory to modify", cxxopts::value <std::string>()->default_value(""))
		("r,remove", "Remove animation")
		("n,name", "Filenames of entities to modify", cxxopts::value<std::vector<std::string>>()->default_value(""));

	options.allow_unrecognised_options();
	auto result = options.parse(argc, argv);

	//if arguments are invalid, print help message
	if (!result.count("animation") || result.count("help"))
	{
		std::cout << options.help() << std::endl;
		return;
	}

	std::vector<entity> entities = get_valid_entities(result["directory"].as<std::string>(), result["family"].as<std::vector<std::string>>(), result["name"].as<std::vector<std::string>>());

	std::vector<std::string> animations = result["animation"].as<std::vector<std::string>>();
	for (auto& animation : animations)
	{
		utilities::name anim_name = utilities::name(animation);

		animation = "animation." + anim_name.space;
		if (result.count("controller"))
		{
			animation = "controller." + animation;
		}
	}

	//add animation to entity list
	if (!result.count("remove"))
	{
		for (auto& ent : entities)
		{
			for (const auto& animation : animations)
			{
				ent.add_animation(animation, result.count("script"));
			}
			ent.write_entity(result["indent"].as<int>());
		}
		return;
	}

	//remove animation from entity list
	for (auto& ent : entities)
	{
		for (const auto& animation : animations)
		{
			ent.remove_animation(animation);
		}
		ent.write_entity(result["indent"].as<int>());
	}
}

void entity::properties(int argc, char* argv[])
{
	//parse arguments
	cxxopts::Options options("prop", "Attaches properties to the entites");
	options.add_options()
		("h,help", "View help")
		("p,property", "Property to add as 'namespace:property'", cxxopts::value<std::string>())
		("i,indent", "JSON file indent", cxxopts::value<int>()->default_value("4"))
		("f,family", "Family types to modify", cxxopts::value<std::vector<std::string>>()->default_value(""))
		("d,directory", "Subdirectory to modify", cxxopts::value <std::string>()->default_value(""))
		("t,type", "Property type, valid values are: bool, enum, float, int", cxxopts::value <std::string>()->default_value("bool"))
		("v,values", "Possible values, seperated by commas", cxxopts::value<std::vector<std::string>>())
		("default", "Default value", cxxopts::value <std::string>())
		("s,sync", "Client sync")
		("n,name", "Filenames of entities to modify", cxxopts::value<std::vector<std::string>>()->default_value(""));

	options.allow_unrecognised_options();
	auto result = options.parse(argc, argv);

	//if arguments are invalid, print help message
	if (!result.count("property") || result.count("help"))
	{
		std::cout << options.help() << std::endl;
		return;
	}

	std::string property_name = result["property"].as<std::string>();

	std::vector<entity> entities = get_valid_entities(result["directory"].as<std::string>(), result["family"].as<std::vector<std::string>>(), result["name"].as<std::vector<std::string>>());

	//remove component group from entity list
	for (auto& ent : entities)
	{
		std::vector<std::string> values;
		if (result.count("values"))
		{
			values = result["values"].as<std::vector<std::string>>();
		}
		std::string default_value = "";
		if (result.count("default"))
		{
			default_value = result["default"].as<std::string>();
		}

		ent.add_property(property_name, result["type"].as<std::string>(), values, default_value, result.count("sync") >= 1);
		ent.write_entity(result["indent"].as<int>());
	}
}

void entity::property_event(int argc, char* argv[])
{
	//parse arguments
	cxxopts::Options options("eprop", "Adds property event to entities");
	options.add_options()
		("h,help", "View help")
		("p,property", "Property to add as 'namespace:property'", cxxopts::value<std::string>())
		("i,indent", "JSON file indent", cxxopts::value<int>()->default_value("4"))
		("f,family", "Family types to modify", cxxopts::value<std::vector<std::string>>()->default_value(""))
		("d,directory", "Subdirectory to modify", cxxopts::value <std::string>()->default_value(""))
		("v,value", "New value", cxxopts::value<std::string>())
		("e,event_name", "Name of the event. Defaults to 'set_(property)_to_(value)", cxxopts::value<std::string>())
		("n,name", "Filenames of entities to modify", cxxopts::value<std::vector<std::string>>()->default_value(""));

	options.allow_unrecognised_options();
	auto result = options.parse(argc, argv);

	//if arguments are invalid, print help message
	if (!result.count("property") || !result.count("value") || result.count("help"))
	{
		std::cout << options.help() << std::endl;
		return;
	}

	std::string property_name = result["property"].as<std::string>();

	std::vector<entity> entities = get_valid_entities(result["directory"].as<std::string>(), result["family"].as<std::vector<std::string>>(), result["name"].as<std::vector<std::string>>());

	std::string event_name;
	std::string value = result["value"].as<std::string>();

	if (result.count("event_name"))
	{
		event_name = result["event_name"].as<std::string>();
	}
	else
	{
		std::string property_no_namespace = utilities::split(property_name, ":").back();
		event_name = "set_" + property_no_namespace + "_to_" + std::regex_replace(value, std::regex("[^a-zA-Z_:0-9\\+-]"), "_");
	}

	//remove component group from entity list
	for (auto& ent : entities)
	{
		if (ent.add_property_event(property_name, value, event_name))
		{
			ent.write_entity(result["indent"].as<int>());
		}
	}
}

void entity::damage_sensor(int argc, char* argv[])
{
	//parse arguments
	cxxopts::Options options("dmgs", "Attaches or removes damage sensor entries for the entities");
	options.add_options()
		("h,help", "View help")
		("s,sensor", "The damage sensor to add/remove, either a name or JSON string", cxxopts::value<std::string>())
		("i,indent", "JSON file indent", cxxopts::value<int>()->default_value("4"))
		("f,family", "Family types to modify", cxxopts::value<std::vector<std::string>>()->default_value(""))
		("d,directory", "Subdirectory to modify", cxxopts::value <std::string>()->default_value(""))
		("n,name", "Filenames of entities to modify, i.e. player.json", cxxopts::value<std::vector<std::string>>()->default_value(""));

	options.allow_unrecognised_options();
	auto result = options.parse(argc, argv);

	//if arguments are invalid, print help message
	if (!result.count("sensor") || result.count("help"))
	{
		std::cout << options.help() << std::endl;
		return;
	}

	nlohmann::ordered_json sensor;
	try
	{
		sensor = nlohmann::ordered_json::parse(result["sensor"].as<std::string>());
	}
	catch (const std::exception&)
	{
		sensor[result["sensor"].as<std::string>()] = nlohmann::json::object();
	}

	std::vector<entity> entities = get_valid_entities(result["directory"].as<std::string>(), result["family"].as<std::vector<std::string>>(), result["name"].as<std::vector<std::string>>());

	for (auto& ent : entities)
	{
		ent.add_damage_sensor(sensor);
		ent.write_entity(result["indent"].as<int>());
	}
}

std::vector<entity::entity> entity::get_valid_entities(std::string directory, std::vector<std::string> families, std::vector<std::string> names)
{
	std::vector<entity> entities;

	for (const auto& file : file_manager::get_files_in_directory(file_manager::get_bp_path() + "\\entities\\" + directory, names))
	{
		if (families.empty() || families[0].empty())
		{
			entities.push_back(entity(file));
		}

		bool contains_family_keyword = false;
		for (const auto& family : families)
		{
			if (file_manager::read_file(file).find(family) != std::string::npos)
			{
				contains_family_keyword = true;
				continue;
			}
			contains_family_keyword = false;
		}

		if (contains_family_keyword)
		{
			entity ent = entity(file);
			if (ent.contains_family_type(families))
			{
				entities.push_back(ent);
			}
		}
	}

	return entities;
}

void entity::add_custom_weapon_entry(const std::string& weapon_name, const int& indent)
{
	std::string shortname = utilities::split(weapon_name, ':').back();

	// Player Entity
	nlohmann::ordered_json rp_entity = file_manager::read_json_from_web_page("https://raw.githubusercontent.com/Mojang/bedrock-samples/main/resource_pack/entity/player.entity.json");
	rp_entity = file_manager::read_json_from_file(file_manager::get_rp_path() + "\\entity\\player.entity.json", rp_entity);
	rp_entity["minecraft:client_entity"]["description"]["animations"]["ctrl." + shortname] = "controller.animation.player.custom_weapons." + shortname;
	rp_entity["minecraft:client_entity"]["description"]["animations"][shortname + ".idle.first_person"] = "animation.player." + shortname + ".idle.first_person";
	rp_entity["minecraft:client_entity"]["description"]["animations"][shortname + ".idle.third_person"] = "animation.player." + shortname + ".idle.third_person";
	rp_entity["minecraft:client_entity"]["description"]["animations"][shortname + ".attack.first_person"] = "animation.player." + shortname + ".attack.first_person";
	rp_entity["minecraft:client_entity"]["description"]["animations"][shortname + ".attack.third_person"] = "animation.player." + shortname + ".attack.third_person";

	//handle variables
	std::vector<std::string> pre_anim = rp_entity["minecraft:client_entity"]["description"]["scripts"]["pre_animation"];
	std::string has_custom = "v.has_custom_weapon = v." + shortname + ";";
	size_t index = 10;
	for (size_t i = 0; i < pre_anim.size(); i++)
	{
		if (pre_anim[i].find("has_custom_weapon") != std::string::npos)
		{
			has_custom = pre_anim[i];
			utilities::replace_all(has_custom, ";", "");
			has_custom += " || v." + shortname + ";";
			pre_anim[i] = has_custom;
			index = i;
		}
	}
	pre_anim.insert(pre_anim.begin() + index, "v." + shortname + " = q.is_item_name_any('slot.weapon.mainhand', 0, '" + weapon_name + "');");
	rp_entity["minecraft:client_entity"]["description"]["scripts"]["pre_animation"] = pre_anim;

	file_manager::write_json_to_file(rp_entity, file_manager::get_rp_path() + "\\entity\\player.entity.json", indent);

	// Player Anim Controller
	nlohmann::ordered_json rp_anim_controller = file_manager::read_json_from_file(file_manager::get_rp_path() + "\\animation_controllers\\player.ac.json", rp_player_ac_default);
	rp_anim_controller["animation_controllers"]["controller.animation.player.custom_weapon.select"]["states"]["no_weapon"]["transitions"].push_back({ { shortname, "v." + shortname } });
	std::vector<nlohmann::ordered_json> transitions = rp_anim_controller["animation_controllers"]["controller.animation.player.custom_weapon.select"]["states"]["no_weapon"]["transitions"];
	transitions.insert(transitions.begin(), { {{"no_weapon", "!v.has_custom_weapon"}} });
	rp_anim_controller["animation_controllers"]["controller.animation.player.custom_weapon.select"]["states"][shortname]["animations"].push_back("ctrl." + shortname);
	rp_anim_controller["animation_controllers"]["controller.animation.player.custom_weapon.select"]["states"][shortname]["transitions"] = transitions;

	for (auto& item : rp_anim_controller["animation_controllers"]["controller.animation.player.custom_weapon.select"]["states"].items())
	{
		if (item.key() != "no_weapon")
		{
			size_t new_weapon = 0;
			auto temp_transitions = transitions;
			for (size_t i = 0; i < transitions.size(); i++)
			{
				if (transitions[i].contains(item.key()))
				{
					new_weapon = i;
					break;
				}
			}
			temp_transitions.erase(temp_transitions.begin() + new_weapon);
			item.value()["transitions"] = temp_transitions;
		}
	}

	std::string json_string = R"({ "initial_state": "idle", "states": { "idle": { "animations": [ { "example_weapon.idle.first_person": "v.is_first_person" }, { "example_weapon.idle.third_person": "!v.is_first_person" } ], "transitions": [ { "attack": "v.attack_time > 0" } ], "blend_transition": 0.2 }, "attack": { "animations": [ { "example_weapon.attack.first_person": "v.is_first_person" }, { "example_weapon.attack.third_person": "!v.is_first_person" } ], "transitions": [ { "idle": "q.any_animation_finished" } ], "blend_transition": 0.2 } } })";
	utilities::replace_all(json_string, "example_weapon", shortname);

	rp_anim_controller["animation_controllers"]["controller.animation.player.custom_weapons." + shortname] = nlohmann::ordered_json::parse(json_string);
	file_manager::write_json_to_file(rp_anim_controller, file_manager::get_rp_path() + "\\animation_controllers\\player.ac.json", indent);

	// Player Animation
	nlohmann::ordered_json rp_anim = file_manager::read_json_from_file(file_manager::get_rp_path() + "\\animations\\player.anim.json", rp_player_anim);
	if(!rp_anim["animations"].contains("animation.player." + shortname + ".idle.first_person")) rp_anim["animations"]["animation.player." + shortname + ".idle.first_person"] = nlohmann::json::object();
	if(!rp_anim["animations"].contains("animation.player." + shortname + ".idle.third_person")) rp_anim["animations"]["animation.player." + shortname + ".idle.third_person"] = nlohmann::json::object();
	if(!rp_anim["animations"].contains("animation.player." + shortname + ".attack.first_person")) rp_anim["animations"]["animation.player." + shortname + ".attack.first_person"] = nlohmann::json::object();
	if(!rp_anim["animations"].contains("animation.player." + shortname + ".attack.third_person")) rp_anim["animations"]["animation.player." + shortname + ".attack.third_person"] = nlohmann::json::object();
	file_manager::write_json_to_file(rp_anim, file_manager::get_rp_path() + "\\animations\\player.anim.json", indent);
}

entity::entity::entity() : entity_json(nlohmann::ordered_json()) {}

entity::entity::entity(const nlohmann::ordered_json& entity) : entity_json(entity) {}

entity::entity::entity(const std::string& file) : filepath(file)
{
	entity_json = file_manager::read_json_from_file(file, nlohmann::ordered_json());
}

entity::entity::entity(const nlohmann::ordered_json& entity, const std::string& file) : entity_json(entity), filepath(file) {}

entity::entity::~entity() {}

const bool entity::entity::contains_family_type(const std::vector<std::string>& families)
{
	std::vector<std::string> ent_family = entity_json["minecraft:entity"]["components"]["minecraft:type_family"]["family"];
	std::vector<std::string> search_family = families;

	std::sort(ent_family.begin(), ent_family.end());
	std::sort(search_family.begin(), search_family.end());

	return std::includes(ent_family.begin(), ent_family.end(), search_family.begin(), search_family.end());
}

void entity::entity::add_property(const std::string& property_name, const std::string& type, const std::vector<std::string>& values, const std::string& default_value, const bool& sync)
{
	std::vector<std::string> command_list{ "bool", "enum", "float", "int" };
	auto it = std::find(command_list.begin(), command_list.end(), type);
	int index = std::distance(command_list.begin(), it);

	switch (index)
	{
	case 0: //bool
		entity_json["minecraft:entity"]["description"]["properties"][property_name]["type"] = "bool";
		for (const auto& val : values)
		{
			bool is_true = utilities::to_lower(val) == "true" || val == "1";
			if (is_true || (utilities::to_lower(val) == "false" || val == "0"))
			{
				entity_json["minecraft:entity"]["description"]["properties"][property_name]["values"].push_back(is_true);
			}
			else
			{
				entity_json["minecraft:entity"]["description"]["properties"][property_name]["values"].push_back(val);
			}
		}
		if (!default_value.empty())
		{
			bool is_true = (utilities::to_lower(default_value) == "true" || default_value == "1");
			if (is_true || (utilities::to_lower(default_value) == "false" || default_value == "0"))
			{
				entity_json["minecraft:entity"]["description"]["properties"][property_name]["default"] = is_true;
			}
			else
			{
				entity_json["minecraft:entity"]["description"]["properties"][property_name]["default"] = default_value;
			}
		}
		break;
	case 1: //enum
		entity_json["minecraft:entity"]["description"]["properties"][property_name]["type"] = "enum";
		for (const auto& val : values)
		{
			entity_json["minecraft:entity"]["description"]["properties"][property_name]["values"].push_back(val);
		}
		if (!default_value.empty())
		{
			entity_json["minecraft:entity"]["description"]["properties"][property_name]["default"] = default_value;
		}
		break;
	case 2: //float
		entity_json["minecraft:entity"]["description"]["properties"][property_name]["type"] = "float";
		if (!values.empty())
		{
			try
			{
				entity_json["minecraft:entity"]["description"]["properties"][property_name]["range"].push_back(std::stod(values.front()));
				entity_json["minecraft:entity"]["description"]["properties"][property_name]["range"].push_back(std::stod(values.back()));
			}
			catch (const std::exception&)
			{
				entity_json["minecraft:entity"]["description"]["properties"][property_name]["range"].push_back(values.front());
				entity_json["minecraft:entity"]["description"]["properties"][property_name]["range"].push_back(values.back());
			}
		}
		if (!default_value.empty())
		{
			try
			{
				entity_json["minecraft:entity"]["description"]["properties"][property_name]["default"] = std::stod(default_value);
			}
			catch (const std::exception&)
			{
				entity_json["minecraft:entity"]["description"]["properties"][property_name]["default"] = default_value;
			}
		}
		break;
	case 3: //int
		entity_json["minecraft:entity"]["description"]["properties"][property_name]["type"] = "int";
		if (!values.empty())
		{
			try
			{
				entity_json["minecraft:entity"]["description"]["properties"][property_name]["range"].push_back(std::stoi(values.front()));
				entity_json["minecraft:entity"]["description"]["properties"][property_name]["range"].push_back(std::stoi(values.back()));
			}
			catch (const std::exception&)
			{
				entity_json["minecraft:entity"]["description"]["properties"][property_name]["range"].push_back(values.front());
				entity_json["minecraft:entity"]["description"]["properties"][property_name]["range"].push_back(values.back());
			}
		}
		if (!default_value.empty())
		{
			try
			{
				entity_json["minecraft:entity"]["description"]["properties"][property_name]["default"] = std::stoi(default_value);
			}
			catch (const std::exception&)
			{
				entity_json["minecraft:entity"]["description"]["properties"][property_name]["default"] = default_value;
			}
		}
		break;
	default:
		std::cerr << "Invalid Type: " << type << ". Valid types are: bool, enum, float, int." << std::endl;
		exit(0);
		break;
	}

	if (sync)
	{
		entity_json["minecraft:entity"]["description"]["properties"][property_name]["client_sync"] = sync;
	}
}

void entity::entity::add_animation(const std::string& anim_name, bool scripts)
{
	std::string short_name = utilities::split(anim_name, '.').back();
	if (anim_name.find("controller") != std::string::npos)
	{
		short_name = "ctrl." + short_name;
	}

	entity_json["minecraft:entity"]["description"]["animations"][short_name] = anim_name;

	if (scripts)
	{
		entity_json["minecraft:entity"]["description"]["scripts"]["animate"].push_back(short_name);
	}
}

void entity::entity::remove_animation(const std::string& anim_name)
{
	std::string short_name = utilities::split(anim_name, '.').back();
	if (anim_name.find("controller") != std::string::npos)
	{
		short_name = "ctrl." + short_name;
	}

	entity_json["minecraft:entity"]["description"]["animations"].erase(short_name);
	nlohmann::json val = entity_json["minecraft:entity"]["description"]["scripts"]["animate"];
	auto result = std::find(val.begin(), val.end(), short_name);
	if (result != val.end())
	{
		int index = result - val.begin();
		entity_json["minecraft:entity"]["description"]["scripts"]["animate"].erase(index);
	}

	if (entity_json["minecraft:entity"]["description"]["scripts"]["animate"].empty())
	{
		entity_json["minecraft:entity"]["description"]["scripts"]["animate"] = nlohmann::json::array();
	}
}

void entity::entity::add_component_group(const nlohmann::ordered_json& component_group)
{
	try
	{
		entity_json["minecraft:entity"]["component_groups"][component_group.items().begin().key()] = component_group.items().begin().value();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void entity::entity::remove_component_group(const std::string& group_name)
{
	entity_json["minecraft:entity"]["component_groups"].erase(group_name);
}

void entity::entity::add_component(const nlohmann::ordered_json& component)
{
	try
	{
		entity_json["minecraft:entity"]["components"][component.items().begin().key()] = component.items().begin().value();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void entity::entity::remove_component(const std::string& component_name)
{
	entity_json["minecraft:entity"]["components"].erase(component_name);
}

void entity::entity::add_damage_sensor(const nlohmann::ordered_json& sensor)
{
	try
	{
		if (entity_json["minecraft:entity"]["components"]["minecraft:damage_sensor"]["triggers"].is_object())
		{
			nlohmann::ordered_json old_object = entity_json["minecraft:entity"]["components"]["minecraft:damage_sensor"]["triggers"];
			entity_json["minecraft:entity"]["components"]["minecraft:damage_sensor"]["triggers"] = nlohmann::json::array();
			entity_json["minecraft:entity"]["components"]["minecraft:damage_sensor"]["triggers"].push_back(old_object);
		}
	}
	catch (const std::exception&)
	{
		entity_json["minecraft:entity"]["components"]["minecraft:damage_sensor"]["triggers"] = nlohmann::json::array();
	}

	try
	{
		entity_json["minecraft:entity"]["components"]["minecraft:damage_sensor"]["triggers"].push_back(sensor);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void entity::entity::remove_event(const std::string& name)
{
	entity_json["minecraft:entity"]["events"].erase("add_" + name);
	entity_json["minecraft:entity"]["events"].erase("remove_" + name);
}

void entity::entity::add_event(const std::string& event_name, bool remove_event)
{
	entity_json["minecraft:entity"]["events"]["add_" + event_name] = { {"add", {{"component_groups", {event_name}}}} };
	if (remove_event)
	{
		entity_json["minecraft:entity"]["events"]["remove_" + event_name] = { {"remove", {{"component_groups", {event_name}}}} };
	}
}

bool entity::entity::add_property_event(const std::string& property_name, const std::string& new_value, const std::string& event_name)
{
	std::string property_type = "";
	try
	{
		property_type = entity_json["minecraft:entity"]["description"]["properties"][property_name]["type"];
	}
	catch (const std::exception&)
	{
		std::cerr << "Entity at: " << filepath << " doesn't contain property: " << property_name << std::endl;
		return false;
	}

	std::vector<std::string> command_list{ "bool", "enum", "float", "int" };
	auto it = std::find(command_list.begin(), command_list.end(), property_type);
	int index = std::distance(command_list.begin(), it);

	bool is_true = utilities::to_lower(new_value) == "true" || utilities::to_lower(new_value) == "1";

	switch (index)
	{
	case 0: //bool
		if (is_true || utilities::to_lower(new_value) == "false" || utilities::to_lower(new_value) == "0")
		{
			entity_json["minecraft:entity"]["events"][event_name] = { {"set_property", {{property_name, is_true}}} };
		}
		else
		{
			entity_json["minecraft:entity"]["events"][event_name] = { {"set_property", {{property_name, new_value}}} };
		}
		break;
	case 1: //enum
		entity_json["minecraft:entity"]["events"][event_name] = { {"set_property", {{property_name, new_value}}} };
		break;
	case 2: //float
		try
		{
			entity_json["minecraft:entity"]["events"][event_name] = { {"set_property", {{property_name, std::stod(new_value)}}} };
		}
		catch (const std::exception&)
		{
			entity_json["minecraft:entity"]["events"][event_name] = { {"set_property", {{property_name, new_value}}} };
		}
		break;
	case 3: //int
		try
		{
			entity_json["minecraft:entity"]["events"][event_name] = { {"set_property", {{property_name, std::stoi(new_value)}}} };
		}
		catch (const std::exception&)
		{
			entity_json["minecraft:entity"]["events"][event_name] = { {"set_property", {{property_name, new_value}}} };
		}
		break;
	default:
		break;
	}

	return true;
}

void entity::entity::write_entity(int indent)
{
	if (!filepath.empty())
	{
		file_manager::write_json_to_file(entity_json, filepath, indent);
	}
}

const nlohmann::ordered_json entity::entity::get_json()
{
	return entity_json;
}
