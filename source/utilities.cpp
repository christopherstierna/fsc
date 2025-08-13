#include <iostream>
#include <string>
#include <algorithm>
#include <filesystem>
#include <stdexcept>
#include <sstream>
#include <fstream>

#include "argument_parser.hpp"

namespace fsc_utilities
{
    bool PromptConfirmation(const std::string& prompt)
    {
        std::string input;
        while (true)
        {
            std::cout << prompt << " (y/n): ";
            std::getline(std::cin, input);
            std::transform(input.begin(), input.end(), input.begin(), ::tolower);
            if (input == "y" || input == "yes")
            {
                return true;
            }
            else if (input == "n" || input == "no")
            {
                return false;
            }
            else
            {
                std::cout << "Invalid input. Please enter 'y' or 'n'.\n";
            }
        }
    }

    std::string ReadFile(std::filesystem::path path)
    {
        std::ifstream file{ path };
        if (!file || !file.is_open())
        {
            throw std::runtime_error{ "Failed to read file \"" + path.string() + "\"." };
        }

        std::ostringstream ss;
        ss << file.rdbuf();
        return ss.str();
    }

    bool ValidateMove(std::filesystem::path target, std::filesystem::path destination, bool overwriteFlag, bool silentPromptFlag)
    {
        if (!std::filesystem::exists(target))
        {
            throw std::runtime_error{ "Target does not exist." };
        }
        target = std::filesystem::canonical(target);

        if (!std::filesystem::exists(destination))
        {
            throw std::runtime_error{ "Destination does not exist." };
        }
        destination = std::filesystem::canonical(destination);

        if (target == destination)
        {
            throw std::runtime_error{ "Target and destination are the same path." };
        }

        if (!std::filesystem::is_directory(destination))
        {
            throw std::runtime_error{ "Destination is not a directory." };
        }

        if (target.parent_path() == destination)
        {
            throw std::runtime_error{ "Target is already in destination." };
        }

        std::string targetName{ target.filename().string() };
        bool overwrite{ std::filesystem::exists(destination / targetName) };
        if (overwrite)
        {
            if (!overwriteFlag)
            {
                throw std::runtime_error{ "Destination already has item \"" + targetName + "\", use flag \"-o\" to overwrite." };
            }
            else if (!silentPromptFlag)
            {
                if (!fsc_utilities::PromptConfirmation("Overwrite item? \"" + std::filesystem::canonical((destination / targetName)).string() + "\"."))
                {
                    return false;
                }
            }
        }

        return true;
    }
}