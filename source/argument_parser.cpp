#include <iostream>
#include <stdexcept>
#include <algorithm>

#include "argument_parser.hpp"
#include "command_list.hpp"
#include "command_structure.hpp"

ArgumentParser::ArgumentParser(int argc, char* argv[], const CommandList& commandList)
{
    if (argc == 1)
    {
        throw std::runtime_error{ "No command provided, see \"help\"." };
    }

    command = argv[1];

    if (!commandList.CommandStructureExists(command))
    {
        throw std::runtime_error{ "Unknown command, see \"help\"." };
    }

    const CommandStructure& commandStructure{ commandList.GetCommandStructure(command) };
    std::size_t parameterIndex{ 0 };
    std::size_t argumentsReceived{ 0 };

    for (std::size_t i{ 2 }; i < static_cast<std::size_t>(argc); ++i)
    {
        std::string argument{ argv[i] };
        if (commandStructure.parameters.size() > parameterIndex)
        {
            if (argument[0] != '-')
            {
                arguments[commandStructure.parameters[parameterIndex].name] = argument;
                argumentsReceived += 1;
            }
            else
            {
                if (commandStructure.parameters[parameterIndex].requirement == ParameterRequirement::OPTIONAL)
                {
                    AddFlag(argument, commandStructure);
                }
                else
                {
                    throw std::runtime_error{ "Expected parameter \"" + commandStructure.parameters[parameterIndex].name + "\", instead got flag \"" + argument + "\"." };
                }
            }
            parameterIndex += 1;
        }
        else
        {
            AddFlag(argument, commandStructure);
        }
    }

    std::size_t argumentsRequired{ 0 };
    for (const Parameter& parameter : commandStructure.parameters)
    {
        if (parameter.requirement == ParameterRequirement::REQUIRED)
        {
            argumentsRequired += 1;
        }
    }

    if (argumentsReceived < argumentsRequired)
    {
        std::string errorMessage;
        errorMessage += "Missing arguments: ";
        for (std::size_t i{ argumentsReceived }; i < argumentsRequired;)
        {
            if (commandStructure.parameters[i].requirement == ParameterRequirement::REQUIRED)
            {
                errorMessage += commandStructure.parameters[i].name + " ";
                ++i;
            }
        }
        throw std::runtime_error{ errorMessage };
    }
}

std::string ArgumentParser::GetCommand() const noexcept
{
    return command;
}

bool ArgumentParser::HasArgument(const std::string& parameterName) const noexcept
{
    return arguments.contains(parameterName);
}

std::string ArgumentParser::GetArgument(const std::string& parameterName) const noexcept
{
    return arguments.at(parameterName);
}

bool ArgumentParser::HasFlag(const std::string& targetFlagName) const noexcept
{
    auto it{ std::find_if(flags.begin(), flags.end(),
        [&targetFlagName](const std::string& flagName)
        {
            return targetFlagName == flagName;
        }
    ) };
    return it != flags.end();
}

void ArgumentParser::AddFlag(const std::string& argument, const CommandStructure& commandStructure)
{
    auto it{ std::find_if(commandStructure.flags.begin(), commandStructure.flags.end(),
        [&argument](const Flag& flag)
        {
            return flag.name == argument;
        }
    ) };
    if (it == commandStructure.flags.end())
    {
        throw std::runtime_error{ "Command \"" + commandStructure.name + "\" does not accept flag \"" + argument + "\"." }; 
    }
    flags.push_back(argument);
}