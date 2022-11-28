#include "include/program_arguments.hpp"

namespace
{
	static std::function<void(int argc, char* argv[])> _command = help::output_help;
	static std::string _help = "help";
	static std::string _version_number = "0.1.2";
}

void program_arguments::parse(int argc, char* argv[])
{
	cxxopts::Options options("bed", "Command line tool to help create bedrock addons");
	options.add_options()
		("h,help", "View help")
		("v,version", "Show version number")
		("bpath", "The path to the behavior pack root", cxxopts::value<std::string>())
		("rpath", "The path to the resource pack root", cxxopts::value<std::string>())
		("command", "The command to perform", cxxopts::value<std::string>());
	options.parse_positional({"command"});
	options.allow_unrecognised_options();
	options.positional_help("<command>");

	auto result = options.parse(argc, argv);
	_help = options.help();

	if (result.count("version"))
	{
		std::cout << "Version " << _version_number << std::endl;
		exit(0);
	}

	if (result.count("command"))
	{
		assign_command(result["command"].as<std::string>());
	}

	if (result.count("bpath"))
	{
		file_manager::set_bp_path(result["bpath"].as<std::string>());
	}

	if (result.count("rpath"))
	{
		file_manager::set_rp_path(result["rpath"].as<std::string>());
	}
}

void program_arguments::run_command(int argc, char* argv[])
{
	_command(argc, argv);
}

void program_arguments::assign_command(const std::string& arg)
{
	std::vector<std::string> command_list{"cogr", "comp", "nent", "nitm", "nblk", "anim", "ctrl", "eanim", "func", "prop", "eprop" };
	auto it = std::find(command_list.begin(), command_list.end(), arg);

	int index = std::distance(command_list.begin(), it);
	switch (index)
	{
	case 0: //COGR
		_command = entity::component_group;
		break;
	case 1: //COMP
		_command = entity::component;
		break;
	case 2: //NENT
		_command = entity::new_entity;
		break;
	case 3: //NITM
		_command = item::new_item;
		break;
	case 4: //NBLK
		_command = block::new_block;
		break;
	case 5: //ANIM
		_command = animation::new_animation;
		break;
	case 6: //CTRL
		_command = animation::new_controller;
		break;
	case 7: //EANIM
		_command = entity::animation;
		break;
	case 8: //FUNC
		_command = function::new_function;
		break;
	case 9: //PROP
		_command = entity::properties;
		break;
	case 10: //EPROP
		_command = entity::property_event;
		break;
	default: //HELP
		std::cout << "Unrecognized Command: " << arg << std::endl;
		break;
	}
}

std::string program_arguments::get_options_help()
{
	return _help;
}
