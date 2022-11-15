#include "help.hpp"

void help::output_help(int argc, char* argv[])
{
	std::cout << program_arguments::get_options_help() << std::endl;
	std::cout << "Commands:" << std::endl;
	std::cout << "\tcomp: Attaches or removes components for the entites" << std::endl;
	std::cout << "\tcogr: Attaches or removes component groups for the entities" << std::endl;
	std::cout << "\tnent: Create new entities" << std::endl;
	std::cout << "\tnitm: Create new items" << std::endl;
	std::cout << "\tnblk: Create new blocks" << std::endl;
	std::cout << std::endl;
}
