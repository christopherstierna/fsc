#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>

#include "argument_parser.hpp"

ArgumentParser::ArgumentParser(int argumentCount, char* arguments[])
{
    if (argumentCount == 1)
    {
        throw std::runtime_error{ "No command entered. Use \"help\" to see a list of valid commands." };
    }

    command = arguments[1];

    if (command == "help")
    {
        if (argumentCount > 2)
        {
            parameters.emplace_back(arguments[2]);
        }
        return;
    }

    if (command == "create")
    {
        if (argumentCount <= 2)
        {
            throw std::runtime_error{ "Missing parameter (path). Use \"help create\" to see command structure." };
        }
        parameters.emplace_back(arguments[2]);
        for (int i{ 3 }; i < argumentCount; ++i)
        {
            flags.emplace_back(arguments[i]);
        }
        return;
    }

    if (command == "delete")
    {
        if (argumentCount <= 2)
        {
            throw std::runtime_error{ "Missing parameter (path). Use \"help delete\" to see command structure." };
        }
        parameters.emplace_back(arguments[2]);
        for (int i{ 3 }; i < argumentCount; ++i)
        {
            flags.emplace_back(arguments[i]);
        }
        return;
    }

    if (command == "list")
    {
        if (argumentCount > 2)
        {
            parameters.emplace_back(arguments[2]);
        }
        for (int i{ 3 }; i < argumentCount; ++i)
        {
            flags.emplace_back(arguments[i]);
        }
        return;
    }

    if (command == "read")
    {
        if (argumentCount <= 2)
        {
            throw std::runtime_error{ "Missing parameter (path). Use \"help read\" to see command structure." };
        }
        parameters.emplace_back(arguments[2]);
        return;
    }

    throw std::runtime_error{ "\"" + command + "\" is an invalid command. Use \"help\" to see a list of valid commands." };
}

std::string ArgumentParser::GetCommand() const noexcept
{
    return command;
}

bool ArgumentParser::HasParameter(int index) const noexcept
{
    return static_cast<int>(parameters.size()) > index;
}

std::string ArgumentParser::GetParameter(int index) const noexcept
{
    return parameters[static_cast<std::size_t>(index)];
}

bool ArgumentParser::HasFlag(const std::string& flag) const noexcept
{
    return std::find(flags.begin(), flags.end(), flag) != flags.end();
}