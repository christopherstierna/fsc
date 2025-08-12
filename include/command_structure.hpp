#pragma once

#include <string>
#include <vector>
#include <functional>

#include "argument_parser.hpp"

struct Flag
{
    std::string name;
    std::string purpose;
};

enum class ParameterRequirement
{
    REQUIRED,
    OPTIONAL,
};

struct Parameter
{
    std::string name;
    ParameterRequirement requirement;
    std::string purpose;
};

struct CommandStructure
{
    std::string name;
    std::vector<Parameter> parameters;
    std::vector<Flag> flags;
    std::function<void(const ArgumentParser&)> func;
};