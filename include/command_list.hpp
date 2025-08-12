#pragma once

#include <vector>
#include <string>

#include "command_structure.hpp"

class CommandList
{
public:

    void AddCommandStructure(CommandStructure commandStructure) noexcept;
    const std::vector<CommandStructure>& GetCommandStructures() const noexcept;
    bool CommandStructureExists(const std::string& commandName) const noexcept;
    CommandStructure GetCommandStructure(const std::string& commandName) const;

private:

    std::vector<CommandStructure> commandStructures;

};