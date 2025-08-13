#pragma once

#include <string>
#include <filesystem>

class ArgumentParser;

namespace fsc_utilities
{
    bool PromptConfirmation(const std::string& prompt);
    std::string ReadFile(std::filesystem::path path);
    bool ValidateMove(std::filesystem::path target, std::filesystem::path destination, bool overwriteFlag, bool silentPromptFlag);
}