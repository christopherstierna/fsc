#include <iostream>
#include <stdexcept>

#include "argument_parser.hpp"
#include "command_executor.hpp"

int main(int argc, char* argv[])
{
    try
    {
        ArgumentParser argumentParser{ argc, argv };
        CommandExecutor commandExecutor{ argumentParser };
    }
    catch (const std::exception& error)
    {
        std::cerr << error.what() << std::endl;
    }
    return 0;
}