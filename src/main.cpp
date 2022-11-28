// BedrockCLI.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "include/main.hpp"

int main(int argc, char* argv[])
{
    try
    {
        program_arguments::parse(argc, argv);
    }
    catch (const std::exception& x)
    {
        std::cerr << x.what() << std::endl;
        return EXIT_FAILURE;
    }

    program_arguments::run_command(argc, argv);
}
