#pragma once

#include <string>
#include <filesystem>

namespace fsc_utilities
{
    bool PromptConfirmation(const std::string& prompt);
    std::string ReadFile(std::filesystem::path path);
}