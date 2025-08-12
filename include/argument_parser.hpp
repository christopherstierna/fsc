#pragma once

#include <string>
#include <vector>
#include <unordered_map>

struct CommandStructure;
class CommandList;

class ArgumentParser
{
public:

    ArgumentParser(int argc, char* argv[], const CommandList& commandList);

    std::string GetCommand() const noexcept;
    bool HasArgument(const std::string& parameterName) const noexcept;
    std::string GetArgument(const std::string& parameterName) const noexcept;
    bool HasFlag(const std::string& flagName) const noexcept;

private:

    void AddFlag(const std::string& argument, const CommandStructure& commandStructure);

    std::string command;
    std::unordered_map<std::string, std::string> arguments;
    std::vector<std::string> flags;

};