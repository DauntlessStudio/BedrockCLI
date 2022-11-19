#include "function.hpp"

void function::new_function(int argc, char* argv[])
{
	//parse arguments
	cxxopts::Options options("func", "Create new functions");
	options.add_options()
		("h,help", "View help")
		("c,commands", "The command values", cxxopts::value<std::vector<std::string>>()->default_value("say test"))
		("n,name", "Function names `foo/bar`", cxxopts::value<std::vector<std::string>>());

	options.allow_unrecognised_options();
	auto result = options.parse(argc, argv);

	//if arguments are invalid, print help message
	if (!result.count("name") || result.count("help"))
	{
		std::cout << options.help() << std::endl;
		return;
	}

	std::vector<std::string> commands = result["commands"].as<std::vector<std::string>>();

	for (auto& command : commands)
	{
		format_function(command);
		command.push_back('\n');
	}

	std::string contents = utilities::vector_to_string(commands);
	contents.erase(contents.size() - 1);

	std::vector<std::string> files = result["name"].as<std::vector<std::string>>();

	for (auto& file : files)
	{
		utilities::replace_all(file, "/", "\\");
		file_manager::write_file(file_manager::get_bp_path() + "\\functions\\" + file + ".mcfunction", contents);
	}
}

void function::format_function(std::string& function)
{
	std::vector<std::string> functions = utilities::split(function, '\n');
	for (auto& line : functions)
	{
		if (line.front() == '"' && line.size() >= 2)
		{
			line = line.substr(1, line.size() - 2);
		}

		if (line.front() == '/')
		{
			line = line.substr(1, line.size());
		}
	}

	function = utilities::vector_to_string(functions);
}
