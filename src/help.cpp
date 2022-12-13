#include "include/help.hpp"

void help::output_help(int argc, char* argv[])
{
	std::cout << program_arguments::get_options_help() << std::endl;
	std::cout << "Commands:" << std::endl;
	std::cout << "\tcomp: Adds or removes components for the entites" << std::endl;
	std::cout << "\tcogr: Adds or removes component groups for the entities" << std::endl;
	std::cout << "\tdmgs: Adds damage sensor entries for the entities" << std::endl;
	std::cout << "\teanim: Adds or removes animation references for the entities" << std::endl;
	std::cout << "\tprop: Adds properties to the entities" << std::endl;
	std::cout << "\teprop: Adds an event setting property to the entities" << std::endl;
	std::cout << "\tpent: Create player entity" << std::endl;
	std::cout << "\tnent: Create new entities" << std::endl;
	std::cout << "\tnitm: Create new items" << std::endl;
	std::cout << "\tnblk: Create new blocks" << std::endl;
	std::cout << "\tanim: Create new animation" << std::endl;
	std::cout << "\tctrl: Create new animation controller" << std::endl;
	std::cout << "\tfunc: Create new function" << std::endl;
	std::cout << "\tsdef: Create new sound definition entry" << std::endl;
	std::cout << "\tpkg: Package Manager imports from GitHub repository" << std::endl;
	std::cout << std::endl;
}
