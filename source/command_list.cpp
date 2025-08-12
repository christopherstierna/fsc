#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>

#include "command_list.hpp"
#include "command_structure.hpp"

void CommandList::AddCommandStructure(CommandStructure commandStructure) noexcept
{
    commandStructures.push_back(commandStructure);
}

const std::vector<CommandStructure>& CommandList::GetCommandStructures() const noexcept
{
    return commandStructures;
}

bool CommandList::CommandStructureExists(const std::string& commandName) const noexcept
{
    try
    {
        GetCommandStructure(commandName);
    }
    catch (const std::runtime_error& error)
    {
        return false;
    }
    return true;
}

CommandStructure CommandList::GetCommandStructure(const std::string& commandName) const
{
    auto it{ std::find_if(commandStructures.begin(), commandStructures.end(),
        [commandName](const CommandStructure& commandStructure)
        {
            return commandStructure.name == commandName;
        }
    ) };
    if (it == commandStructures.end())
    {
        throw std::runtime_error{ "Failed to get command stucture of command: \"" + commandName + "\"." };
    }
    return *it;
}