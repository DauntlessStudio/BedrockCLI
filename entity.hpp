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

		void add_component_group(const nlohmann::ordered_json& component_group);
		void add_event(const std::string& event_name, bool remove_event);

		void remove_component_group(const std::string& group_name);
		void remove_event(const std::string& name);

		void write_entity(int indent = 4);

		const nlohmann::ordered_json get_json();
	};
}