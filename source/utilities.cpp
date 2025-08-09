#include <iostream>
#include <string>
#include <algorithm>
#include <filesystem>
#include <stdexcept>
#include <sstream>
#include <fstream>

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
}