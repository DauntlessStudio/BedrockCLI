#include "program_arguments.hpp"

namespace
{
	static std::function<void(int argc, char* argv[])> _command = help::output_help;
	static std::string _command_name = "help";
}

void program_arguments::parse(int argc, char* argv[])
{
	cxxopts::Options options("bed", "Command line tool to help create bedrock addons");
	options.add_options()
		("h,help", "Print usage")
		("bpath", "The path to the behavior pack root", cxxopts::value<std::string>())
		("rpath", "The path to the resource pack root", cxxopts::value<std::string>())
		("command", "The command to perform", cxxopts::value<std::string>());
	options.parse_positional({"command"});
	options.allow_unrecognised_options();

	auto result = options.parse(argc, argv);
	_command_name = result["command"].as<std::string>();

	if (result.count("command"))
	{
		assign_command(_command_name);
	}

	//TODO implement bpath and rpath options

	// If the help argument is given we overwrite the assigned _command with help.
	if (result.count("help"))
	{
		_command = help::output_help;
	}
}

void program_arguments::run_command(int argc, char* argv[])
{
	_command(argc, argv);
}

void program_arguments::assign_command(const std::string& arg)
{
	std::vector<std::string> command_list{"cogr", "comp", "nent", "nitm", "nblk"};
	auto it = std::find(command_list.begin(), command_list.end(), arg);

	int index = std::distance(command_list.begin(), it);
	switch (index)
	{
	case 0: //COGR
		std::cout << "Selected COGR" << std::endl;
		break;
	case 1: //COMP
		std::cout << "Selected COMP" << std::endl;
		break;
	case 2: //NENT
		std::cout << "Selected NENT" << std::endl;
		break;
	case 3: //NITM
		std::cout << "Selected NITM" << std::endl;
		_command = new_item::new_item;
		break;
	case 4: //NBLK
		std::cout << "Selected NBLK" << std::endl;
		_command = new_block::new_block;
		break;
	default: //HELP
		std::cout << "INVALID" << std::endl;
		break;
	}
}
