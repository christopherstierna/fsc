#include <iostream>
#include <filesystem>
#include <stdexcept>
#include <vector>
#include <string>
#include <fstream>

#include "command_structure.hpp"
#include "command_list.hpp"
#include "commands.hpp"
#include "utilities.hpp"

namespace fsc
{
    void Help(const ArgumentParser& argumentParser)
    {
        auto OutputCommandStructure = [](const CommandStructure& commandStructure)
        {
            std::cout << "\n";
            std::cout << "Command: " << commandStructure.name << "\n";
            std::cout << "  Parameters:" << "\n";
            if (commandStructure.parameters.size() > 0)
            {
                for (const Parameter& parameter : commandStructure.parameters)
                {
                    std::cout << "    (" << parameter.name << ") purpose: " << parameter.purpose << " required: ";
                    if (parameter.requirement == ParameterRequirement::REQUIRED)
                    {
                        std::cout << "true";
                    }
                    else
                    {
                        std::cout << "false";
                    }
                    std::cout << "\n";
                }
            }
            else
            {
                std::cout << "    None\n";
            }

            std::cout << "\n";
            std::cout << "  Flags:" << "\n";
            if (commandStructure.flags.size() > 0)
            {
                for (const Flag& flag : commandStructure.flags)
                {
                    std::cout << "    " << flag.name << " (" + flag.purpose << ")\n";
                }
            }
            else
            {
                std::cout << "    None\n";
            }
            std::cout << std::flush;
        };

        const CommandList& commandList{ fsc::GetCommandList() };
        if (argumentParser.HasArgument("command"))
        {
            std::string command{ argumentParser.GetArgument("command") };
            if (commandList.CommandStructureExists(command))
            {
                OutputCommandStructure(commandList.GetCommandStructure(command));
            }
            else
            {
                throw std::runtime_error{ "Cannot help with unknown command \"" + command + "\"." };
            }
        }
        else
        {
            std::cout << "Command structure: fsc <command> <parameters> <flags>\n";
            const std::vector<CommandStructure>& allCommandStructures{ commandList.GetCommandStructures() };
            for (const CommandStructure& commandStructure : allCommandStructures)
            {
                OutputCommandStructure(commandStructure);
            }
        }
    }

    void Create(const ArgumentParser& argumentParser)
    {
        std::filesystem::path path{ argumentParser.GetArgument("path") };
        if (std::filesystem::exists(path))
        {
            throw std::runtime_error{ "Path already exists." };
        }
        path = std::filesystem::absolute(path);

        bool fileFlag{ argumentParser.HasFlag("-f") };
        bool directoryFlag{ argumentParser.HasFlag("-d") };
        
        if (fileFlag && directoryFlag)
        {
            throw std::runtime_error{ "Flags \"-f\" and \"-d\" cannot be used at the same time." };
        }

        bool isFile{ false };
        if (path.has_extension())
        {
            if (!directoryFlag)
            {
                isFile = true;
            }
        }
        else
        {
            if (fileFlag)
            {
                isFile = true;
            }
        }

        if (isFile)
        {
            std::filesystem::create_directories(path.parent_path());
            std::ofstream file{ path };
            if (!file || !file.is_open())
            {
                throw std::runtime_error{ "Error creating file." };
            }

            if (!argumentParser.HasFlag("-n"))
            {
                if (path.extension() == ".hpp" || path.extension() == ".inl" || path.extension() == ".ipp" || path.extension() == ".tpp" || path.extension() == ".hxx")
                {
                    file << "#pragma once\n\n";
                }

                if (path.extension() == ".h")
                {
                    std::string macroName{ path.filename().string() };
                    for (char &character : macroName)
                    {
                        character = (character >= 'a' && character <= 'z') ? (character - 'a' + 'A') : character;
                        if (!std::isalnum(static_cast<unsigned char>(character)))
                        {
                            character = '_';
                        }
                    }
                    if (std::isdigit(static_cast<unsigned char>(macroName[0])))
                    {
                        macroName = "_" + macroName;
                    }

                    file << "#ifndef " + macroName + "\n#define " + macroName + "\n\n\n\n#endif";
                }
            }
            std::cout << "Created file \"" << std::filesystem::canonical(path).string() << "\"." << std::endl;
        }
        else
        {
            std::filesystem::create_directories(path);
            std::cout << "Created directory \"" << std::filesystem::canonical(path).string() << "\"." << std::endl;
        }
    }

    void Delete(const ArgumentParser& argumentParser)
    {
        std::filesystem::path path{ argumentParser.GetArgument("path") };
        if (!std::filesystem::exists(path))
        {
            throw std::runtime_error{ "Path does not exists." };
        }
        path = std::filesystem::canonical(path).make_preferred();

        if (std::filesystem::is_directory(path))
        {
            if (std::filesystem::is_empty(path))
            {
                std::error_code error;
                std::filesystem::remove(path, error);
                if (error)
                {
                    throw std::runtime_error{ "Error: " + error.message() };
                }
                std::cout << "Deleted directory \"" + path.string() + "\"." << std::endl;
            }
            else
            {
                if (!argumentParser.HasFlag("-r"))
                {
                    throw std::runtime_error{ "Directory is not empty, use -r flag." };
                }

                if (!argumentParser.HasFlag("-s"))
                {
                    std::string promptMessage;
                    if (argumentParser.HasFlag("-c"))
                    {
                        promptMessage = "Delete contents of directory? \"" + path.string() + "\"";
                    }
                    else
                    {
                        promptMessage = "Delete directory and contents? \"" + path.string() + "\"";
                    }
                    if (!fsc_utilities::PromptConfirmation(promptMessage))
                    {
                        return;
                    }
                }

                std::error_code error;
                if (argumentParser.HasFlag("-c"))
                {
                    for (auto& entry : std::filesystem::directory_iterator(path))
                    {
                        std::filesystem::remove_all(entry.path(), error);
                        if (error)
                        {
                            break;
                        }
                    }
                }
                else
                {
                    std::filesystem::remove_all(path, error);
                }
                if (error)
                {
                    throw std::runtime_error{ "Error: " + error.message() };
                }
                if (argumentParser.HasFlag("-c"))
                {
                    std::cout << "Deleted contents of directory \"" + path.string() + "\"." << std::endl;
                }
                else
                {
                    std::cout << "Deleted directory and contents \"" + path.string() + "\"." << std::endl;
                }
            }
        }
        else
        {
            std::error_code error;
            std::filesystem::remove(path, error);
            if (error)
            {
                throw std::runtime_error{ "Error: " + error.message() };
            }
            std::cout << "Deleted file \"" + path.string() + "\"." << std::endl;
        }
    }

    void List(const ArgumentParser& argumentParser)
    {
        std::filesystem::path path;
        if (argumentParser.HasArgument("path"))
        {
            path = argumentParser.GetArgument("path");
        }
        else
        {
            path = std::filesystem::current_path();
        }

        if (!std::filesystem::exists(path))
        {
            throw std::runtime_error{ "Path does not exist." };
        }
        path = std::filesystem::canonical(path).make_preferred();

        if (!std::filesystem::is_directory(path))
        {
            throw std::runtime_error{ "Specified path is a file." };
        }

        bool filesOnly{ argumentParser.HasFlag("-f") };
        bool directoriesOnly{ argumentParser.HasFlag("-d") };
        
        if (filesOnly && directoriesOnly)
        {
            throw std::runtime_error{ "Flags \"-f\" and \"-d\" cannot be used at the same time." };
        }

        auto ListPath = [filesOnly, directoriesOnly](std::filesystem::path path)
        {
            if (std::filesystem::is_directory(path))
            {
                if (!filesOnly)
                {
                    std::cout << "D: " + path.filename().string() << "\n";
                }
            }
            else
            {
                if (!directoriesOnly)
                {
                    std::cout << "F: " + path.filename().string() << "\n";
                }
            }
        };

        try
        {
            if (argumentParser.HasFlag("-r"))
            {
                for (const auto& entry : std::filesystem::recursive_directory_iterator(path))
                {
                    ListPath(entry.path());    
                }
            }
            else
            {
               for (const auto& entry : std::filesystem::directory_iterator(path))
                {
                    ListPath(entry.path());    
                } 
            }
            std::cout << std::flush;
        }
        catch (const std::filesystem::filesystem_error& error)
        {
            throw std::runtime_error{ std::string{ "Error: " } + error.what() };
        }
    }

    void Read(const ArgumentParser& argumentParser)
    {
        std::filesystem::path path{ argumentParser.GetArgument("path") };
        if (!std::filesystem::exists(path))
        {
            throw std::runtime_error{ "Path does not exist." };
        }

        if (std::filesystem::is_directory(path))
        {
            throw std::runtime_error{ "Specified path is not a file." };
        }

        std::cout << fsc_utilities::ReadFile(path) << std::endl;
    }

    void Clone(const ArgumentParser& argumentParser)
    {
        std::filesystem::path target{ argumentParser.GetArgument("target") };
        std::filesystem::path destination{ argumentParser.GetArgument("destination") };
        bool validated{ fsc_utilities::ValidateMove(target, destination, argumentParser.HasFlag("-o"), argumentParser.HasFlag("-s")) };
        if (!validated)
        {
            return;
        }
        target = std::filesystem::canonical(target);
        destination = std::filesystem::canonical(destination);

        try
        {
            if (std::filesystem::is_directory(target))
            {
                std::filesystem::copy(target, destination / target.filename(), std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing);
            }
            else
            {
                std::filesystem::copy(target, destination, std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing);
            }
            std::cout << "Cloned \"" + target.filename().string() + "\" to \"" + destination.string() + "\"." << std::endl;
        }
        catch (const std::filesystem::filesystem_error& error)
        {
            throw std::runtime_error{ std::string{ "Error: " } + error.what() };
        }
    }

    void Move(const ArgumentParser& argumentParser)
    {
        std::filesystem::path target{ argumentParser.GetArgument("target") };
        std::filesystem::path destination{ argumentParser.GetArgument("destination") };
        bool validated{ fsc_utilities::ValidateMove(target, destination, argumentParser.HasFlag("-o"), argumentParser.HasFlag("-s")) };
        if (!validated)
        {
            return;
        }
        target = std::filesystem::canonical(target);
        destination = std::filesystem::canonical(destination);

        try
        {
            std::error_code error;
            std::filesystem::rename(target, destination, error);
            if (error)
            {
                if (std::filesystem::is_directory(target))
                {
                    std::filesystem::copy(target, destination / target.filename(), std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing);
                }
                else
                {
                    std::filesystem::copy(target, destination, std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing);
                }
                std::filesystem::remove_all(target);
            }
            std::cout << "Moved \"" + target.filename().string() + "\" to \"" + destination.string() + "\"." << std::endl;
        }
        catch (const std::filesystem::filesystem_error& error)
        {
            throw std::runtime_error{ std::string{ "Error: " } + error.what() };
        }
    }

    void Rename(const ArgumentParser& argumentParser)
    {
        std::filesystem::path target{ argumentParser.GetArgument("target") };
        std::string newName{ argumentParser.GetArgument("new name") };

        if (!std::filesystem::exists(target))
        {
            throw std::runtime_error{ "Target does not exist." };
        }
        target = std::filesystem::canonical(target);

        std::filesystem::path newPath{ target.parent_path() / newName };
        bool itemWithNameAlreadyExists{ std::filesystem::exists(newPath) };

        if (itemWithNameAlreadyExists)
        {
            newPath = std::filesystem::canonical(newPath);
            if (!argumentParser.HasFlag("-o"))
            {
                throw std::runtime_error{ "Item with name \"" + newName + "\" already exists. Use flag \"-o\" to overwrite." };
            }
            else if (!argumentParser.HasFlag("-s"))
            {
                if (!fsc_utilities::PromptConfirmation("Overwrite item? \"" + std::filesystem::canonical(newPath).string() + "\"."))
                {
                    return;
                }
            }
        }

        try
        {
            std::filesystem::rename(target, newPath); 
            std::cout << "Renamed \"" + target.string() + "\" to " + newPath.filename().string() << "\".";
        }
        catch (const std::runtime_error& error)
        {
            throw std::runtime_error{ std::string{ "Error: " } + error.what() };
        }
    }

    void Version(const ArgumentParser&)
    {
        std::cout << "fsc version: 1.0.0" << std::endl;
    }
}