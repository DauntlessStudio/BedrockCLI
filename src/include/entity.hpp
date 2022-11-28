#pragma once
#include "extern\cxxopts\include\cxxopts.hpp"
#include "extern\nlohmann\single_include\nlohmann\json.hpp"
#include "help.hpp"
#include "file_manager.hpp"
#include "utilities.hpp"

namespace entity
{
	void new_entity(int argc, char* argv[]);
	void component_group(int argc, char* argv[]);
	void component(int argc, char* argv[]);
	void animation(int argc, char* argv[]);
	void properties(int argc, char* argv[]);
	void property_event(int argc, char* argv[]);
	void damage_sensor(int argc, char* argv[]);

	class entity
	{
		std::string filepath;
		nlohmann::ordered_json entity_json;

	public:
		entity();
		entity(const nlohmann::ordered_json& entity);
		entity(const std::string& file);
		entity(const nlohmann::ordered_json& entity, const std::string& file);

		~entity();

		const bool contains_family_type(const std::string& family);
		const bool contains_family_type(const std::vector<std::string>& families);

		void add_property(const std::string& property_name, const std::string& type, const std::vector<std::string>& values, const std::string& default_value, const bool& sync);

		void add_animation(const std::string& anim_name, bool scripts);
		void remove_animation(const std::string& anim_name);

		void add_component_group(const nlohmann::ordered_json& component_group);
		void remove_component_group(const std::string& group_name);

		void add_component(const nlohmann::ordered_json& component);
		void remove_component(const std::string& component_name);

		void add_damage_sensor(const nlohmann::ordered_json& sensor);

		void add_event(const std::string& event_name, bool remove_event);
		void remove_event(const std::string& name);

		bool add_property_event(const std::string& property_name, const std::string& new_value);

		void write_entity(int indent = 4);

		const nlohmann::ordered_json get_json();
	};

	std::vector<entity> get_valid_entities(std::string directory, std::vector<std::string> families, std::vector<std::string> names);
}