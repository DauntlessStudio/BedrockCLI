#include "program_arguments.hpp"

namespace
{
	static std::function _command = help::output_help;
	static std::string_view _command_name = "help";
	static std::vector<std::string_view> _filenames;
	static int _indent_level = 4;
	static std::string_view _directory;
	static bool _should_remove = false;
	static std::string_view _family_type;
}

void program_arguments::parse(int argc, char* argv[])
{
	std::vector<std::string_view> command_list{ "cogr", "comp", "nent", "nblk", "dmgs" };
	const std::vector<std::string_view> args(argv + 1, argv + argc);

	for (const auto& arg : args)
	{
		auto found_command = std::find(command_list.begin(), command_list.end(), arg);
		if (found_command != command_list.end())
		{
			if (_command_name != "help")
			{
				throw std::runtime_error("bed: cannot use multiple commands!");
			}
			assign_command(arg, found_command - command_list.begin());
			continue;
		}
		if (arg == "-d" || arg == "--directory")
		{
			if (!_directory.empty())
			{
				throw std::runtime_error("bed: cannot use -d/--directory twice!");
			}
			_directory = arg;
		}
		if (arg == "-f" || arg == "--family")
		{
			if (!_family_type.empty())
			{
				throw std::runtime_error("bed: cannot use -d/--directory twice!");
			}
			_family_type = arg;
		}
		if (arg == "-i" || arg == "--indent")
		{
			if (!_directory.empty())
			{
				throw std::runtime_error("bed: cannot use -d/--directory twice!");
			}
			_directory = arg;
		}
	}
}

void program_arguments::run_command()
{
	_command();
}

void program_arguments::assign_command(const std::string_view& arg, const int command_index)
{
	_command_name = arg;

	switch (command_index)
	{
	case 0: // COGR
		break;
	case 1: // COMP
		break;
	case 2: // NENT
		break;
	case 3: // NBLK
		break;
	case 4: // DMGS
		break;
	default:
		_command = help::output_help;
		break;
	}
}
