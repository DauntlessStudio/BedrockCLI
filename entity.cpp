#include "entity.hpp"

auto bp_dummy_entity = nlohmann::ordered_json::parse(R"({ "format_version": "1.18.0", "minecraft:entity": { "description": { "identifier": "namespace:name", "is_spawnable": true, "is_summonable": true, "is_experimental": false }, "component_groups": { "instant_despawn": { "minecraft:instant_despawn": {} } }, "components": { "minecraft:type_family": { "family": [ "namespace", "name" ] }, "minecraft:collision_box": { "height": 0, "width": 0 }, "minecraft:physics": { "has_collision": false, "has_gravity": false }, "minecraft:damage_sensor": { "triggers": [ { "on_damage": { "filters": { "test": "has_damage", "value": "void" }, "event": "despawn" }, "deals_damage": false }, { "cause": "all", "deals_damage": false } ] } }, "events": { "despawn": { "add": { "component_groups": [ "instant_despawn" ] } } } } })");
auto bp_passive_entity = nlohmann::ordered_json::parse(R"({ "format_version": "1.18.2", "minecraft:entity": { "description": { "identifier": "namespace:entity", "is_spawnable": true, "is_summonable": true, "is_experimental": false }, "component_groups": { "despawn": { "minecraft:instant_despawn": {} } }, "components": { "minecraft:is_hidden_when_invisible": {}, "minecraft:type_family": { "family": [ "mob", "namespace", "name" ] }, "minecraft:breathable": { "total_supply": 15, "suffocate_time": 0 }, "minecraft:nameable": {}, "minecraft:pushable": { "is_pushable": true, "is_pushable_by_piston": true }, "minecraft:conditional_bandwidth_optimization": {}, "minecraft:movement": { "value": 0.25 }, "minecraft:navigation.walk": { "avoid_damage_blocks": true }, "minecraft:movement.basic": {}, "minecraft:jump.static": {}, "minecraft:can_climb": {}, "minecraft:collision_box": { "width": 0.6, "height": 1.8 }, "minecraft:physics": {}, "minecraft:despawn": { "despawn_from_distance": {} }, "minecraft:behavior.float": { "priority": 2 }, "minecraft:behavior.panic": { "priority": 3, "speed_multiplier": 1.25 }, "minecraft:behavior.random_stroll": { "priority": 7 }, "minecraft:behavior.look_at_player": { "priority": 8, "look_distance": 6.0 }, "minecraft:behavior.random_look_around": { "priority": 9 } }, "events": { "despawn": { "add": { "component_groups": [ "despawn" ] } } } } })");
auto bp_hostile_entity = nlohmann::ordered_json::parse(R"({ "format_version": "1.18.2", "minecraft:entity": { "description": { "identifier": "namespace:entity", "is_spawnable": true, "is_summonable": true, "is_experimental": false }, "component_groups": { "despawn": { "minecraft:instant_despawn": {} } }, "components": { "minecraft:is_hidden_when_invisible": {}, "minecraft:type_family": { "family": [ "namespace", "entity", "mob" ] }, "minecraft:breathable": { "total_supply": 15, "suffocate_time": 0 }, "minecraft:nameable": {}, "minecraft:pushable": { "is_pushable": true, "is_pushable_by_piston": true }, "minecraft:conditional_bandwidth_optimization": {}, "minecraft:movement": { "value": 0.25 }, "minecraft:navigation.walk": { "avoid_damage_blocks": true }, "minecraft:movement.basic": {}, "minecraft:jump.static": {}, "minecraft:can_climb": {}, "minecraft:collision_box": { "width": 0.6, "height": 1.8 }, "minecraft:physics": {}, "minecraft:despawn": { "despawn_from_distance": {} }, "minecraft:attack": { "damage": 3 }, "minecraft:behavior.nearest_prioritized_attackable_target": { "priority": 0, "entity_types":[ { "filters": { "test": "is_family", "value": "player", "subject": "other" } } ] }, "minecraft:behavior.melee_attack": { "priority": 1, "speed_multiplier": 1.2, "reach_multiplier": 2 }, "minecraft:behavior.float": { "priority": 2 }, "minecraft:behavior.panic": { "priority": 3, "speed_multiplier": 1.25 }, "minecraft:behavior.random_stroll": { "priority": 7 }, "minecraft:behavior.look_at_player": { "priority": 8, "look_distance": 6.0 }, "minecraft:behavior.random_look_around": { "priority": 9 } }, "events": { "despawn": { "add": { "component_groups": [ "despawn" ] } } } } })");
auto rp_entity_default = nlohmann::ordered_json::parse(R"({ "format_version": "1.18.0", "minecraft:client_entity": { "description": { "identifier": "namespace:name", "min_engine_version": "1.10.0", "materials": { "default": "entity_alphatest" }, "textures": { "default": "textures/entity/name/default" }, "geometry": { "default": "geometry.name" }, "render_controllers": [ "controller.render.default" ] } } })");
auto rp_geo_default = nlohmann::ordered_json::parse(R"({ "format_version": "1.18.0", "minecraft:geometry": [ { "description": { "identifier": "geometry.name", "texture_width": 16, "texture_height": 16, "visible_bounds_width": 2, "visible_bounds_height": 3, "visible_bounds_offset": [0, 0.5, 0] }, "bones": [ { "name": "body", "pivot": [0, 0, 0], "cubes": [ {"origin": [-8, 0, -8], "size": [16, 16, 16], "uv": [0, 0]} ] } ] } ] })");

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

		std::string filename = utilities::split(name, ':').back();
		std::string entity_namespace = utilities::split(name, ':').front();

		bp_entity["minecraft:entity"]["description"]["identifier"] = name;
		bp_entity["minecraft:entity"]["components"]["minecraft:type_family"]["family"] = {entity_namespace, filename};
		file_manager::write_json_to_file(bp_entity, file_manager::get_bp_path() + "\\entities\\" + filename + ".json", result["indent"].as<int>());

		if (result.count("rp"))
		{
			nlohmann::ordered_json rp_entity = rp_entity_default;
			rp_entity["minecraft:client_entity"]["description"]["identifier"] = name;
			rp_entity["minecraft:client_entity"]["description"]["geometry"]["default"] = "geometry." + filename;
			rp_entity["minecraft:client_entity"]["description"]["textures"]["default"] = "textures/entity/" + filename + "/default";

			file_manager::write_json_to_file(rp_entity, file_manager::get_rp_path() + "\\entity\\" + filename + ".json", result["indent"].as<int>());
		}

		if (result.count("model"))
		{
			nlohmann::ordered_json rp_geo = rp_geo_default;
			rp_geo["minecraft:geometry"][0]["description"]["identifier"] = "geometry." + filename;

			file_manager::write_json_to_file(rp_geo, file_manager::get_rp_path() + "\\models\\entity\\" + filename + ".json", result["indent"].as<int>());
		}

		if (result.count("lang"))
		{
			for (const auto& lang_file : result["lang"].as<std::vector<std::string>>())
			{
				file_manager::add_lang_entry("entity." + name + ".name=" + utilities::format_name(filename), lang_file, "entity names");
				if (result.count("enemy") || result.count("passive"))
				{
					file_manager::add_lang_entry("item.spawn_egg." + name + ".name=Spawn " + utilities::format_name(filename), lang_file, "spawn eggs");
				}
			}
		}
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
		animation = "animation." + animation;
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
		("s,sync", "Client sync")
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

	//remove component group from entity list
	for (auto& ent : entities)
	{
		if (ent.add_property_event(property_name, result["value"].as<std::string>()))
		{
			ent.write_entity(result["indent"].as<int>());
		}
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
				break;
			}
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

entity::entity::entity() : entity_json(nlohmann::ordered_json()) {}

entity::entity::entity(const nlohmann::ordered_json& entity) : entity_json(entity) {}

entity::entity::entity(const std::string& file) : filepath(file)
{
	entity_json = file_manager::read_json_from_file(file, nlohmann::ordered_json());
}

entity::entity::entity(const nlohmann::ordered_json& entity, const std::string& file) : entity_json(entity), filepath(file) {}

entity::entity::~entity() {}

const bool entity::entity::contains_family_type(const std::string& family)
{
	// TODO Search file for string first, only serialize if string is found
	if (entity_json["minecraft:entity"]["components"].contains("minecraft:type_family"))
	{
		if (std::find(entity_json["minecraft:entity"]["components"]["minecraft:type_family"]["family"].begin(), entity_json["minecraft:entity"]["components"]["minecraft:type_family"]["family"].end(), family) != std::end(entity_json["minecraft:entity"]["components"]["minecraft:type_family"]["family"]))
		{
			return true;
		};
	}
	return false;
}

const bool entity::entity::contains_family_type(const std::vector<std::string>& families)
{
	for (const auto& family : families)
	{
		if (contains_family_type(family))
		{
			return true;
		}
	}

	return false;
}

void entity::entity::add_property(const std::string& property_name, const std::string& type, const std::vector<std::string>& values, const std::string& default_value, const bool& sync)
{
	std::vector<std::string> command_list{ "bool", "enum", "float", "int" };
	auto it = std::find(command_list.begin(), command_list.end(), type);
	int index = std::distance(command_list.begin(), it);

	switch (index)
	{
	case 0: //bool
		//TODO turn bool to string if needed
		entity_json["minecraft:entity"]["description"]["properties"][property_name]["type"] = "bool";
		for (const auto& val : values)
		{
			if (bool is_true = (utilities::to_lower(val) == "true" || val == "1") || (utilities::to_lower(val) == "false" || val == "0"))
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
			if (bool is_true = (utilities::to_lower(default_value) == "true" || default_value == "1") || (utilities::to_lower(default_value) == "false" || default_value == "0"))
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
		for (const auto& val : values)
		{
			entity_json["minecraft:entity"]["description"]["properties"][property_name]["values"].push_back(val);
		}
		if (!default_value.empty())
		{
			entity_json["minecraft:entity"]["description"]["properties"][property_name]["default"] = default_value;
		}
		break;
	case 3: //int
		entity_json["minecraft:entity"]["description"]["properties"][property_name]["type"] = "int";
		for (const auto& val : values)
		{
			entity_json["minecraft:entity"]["description"]["properties"][property_name]["values"].push_back(val);
		}
		if (!default_value.empty())
		{
			entity_json["minecraft:entity"]["description"]["properties"][property_name]["default"] = default_value;
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

bool entity::entity::add_property_event(const std::string& property_name, const std::string& new_value)
{
	std::string property_no_namespace = utilities::split(property_name, ":").back();
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

	switch (index)
	{
	case 0: //bool
		if (bool is_true = utilities::to_lower(new_value) == "true" || utilities::to_lower(new_value) == "1" || utilities::to_lower(new_value) == "false" || utilities::to_lower(new_value) == "0")
		{
			entity_json["minecraft:entity"]["events"]["set_" + property_no_namespace + "_" + new_value] = { {"set_property", {{property_name, is_true}}} };
		}
		else
		{
			entity_json["minecraft:entity"]["events"]["set_" + property_no_namespace + "_" + new_value] = { {"set_property", {{property_name, new_value}}} };
		}
		break;
	case 1: //enum
		entity_json["minecraft:entity"]["events"]["set_" + property_no_namespace + "_" + new_value] = { {"set_property", {{property_name, new_value}}} };
		break;
	case 2: //float
		try
		{
			entity_json["minecraft:entity"]["events"]["set_" + property_no_namespace + "_" + new_value] = { {"set_property", {{property_name, std::stod(new_value)}}} };
		}
		catch (const std::exception&)
		{
			entity_json["minecraft:entity"]["events"]["set_" + property_no_namespace + "_" + new_value] = { {"set_property", {{property_name, new_value}}} };
		}
		break;
	case 3: //int
		try
		{
			entity_json["minecraft:entity"]["events"]["set_" + property_no_namespace + "_" + new_value] = { {"set_property", {{property_name, std::stoi(new_value)}}} };
		}
		catch (const std::exception&)
		{
			entity_json["minecraft:entity"]["events"]["set_" + property_no_namespace + "_" + new_value] = { {"set_property", {{property_name, new_value}}} };
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
