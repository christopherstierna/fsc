#pragma once

#include <string>
#include <vector>

class ArgumentParser
{
public:

    ArgumentParser(int argumentCount, char* arguments[]);

    std::string GetCommand() const noexcept;
    bool HasParameter(int index) const noexcept;
    std::string GetParameter(int index) const noexcept;
    bool HasFlag(const std::string& flag) const noexcept;

private:

    std::string command;
    std::vector<std::string> parameters;
    std::vector<std::string> flags;

};