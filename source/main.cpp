#include <iostream>
#include <stdexcept>

#include "commands.hpp"
#include "command_list.hpp"
#include "command_structure.hpp"

int main(int argc, char* argv[])
{
    try
    {
        fsc::InitializeCommands();
        const CommandList& commandList{ fsc::GetCommandList() };
        ArgumentParser argumentParser{ argc, argv, commandList };
        CommandStructure commandStructure{ commandList.GetCommandStructure(argumentParser.GetCommand()) };
        commandStructure.func(argumentParser);
    }
    catch (const std::exception& error)
    {
        std::cerr << error.what() << std::endl;
    }
    return 0;
}