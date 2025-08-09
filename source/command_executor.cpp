#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <fstream>
#include <map>

#include "command_executor.hpp"
#include "argument_parser.hpp"
#include "utilities.hpp"

CommandExecutor::CommandExecutor(ArgumentParser& targetArgumentParser)
    :
    argumentParser{ &targetArgumentParser }
{
    if (argumentParser->GetCommand() == "help")
    {
        Help();
        return;
    }
    if (argumentParser->GetCommand() == "create")
    {
        Create();
        return;
    }
    if (argumentParser->GetCommand() == "delete")
    {
        Delete();
        return;
    }
    if (argumentParser->GetCommand() == "list")
    {
        List();
        return;
    }
    if (argumentParser->GetCommand() == "read")
    {
        Read();
        return;
    }

    throw std::runtime_error{ "\"" + argumentParser->GetCommand() + "\" is an invalid command. Use \"help\" to see a list of valid commands." };
}

void CommandExecutor::Help()
{
    std::map<std::string, std::string> commandHelp;
    commandHelp["Command structure"] =  "Basic command structure: fsc <command> <parameters> <flags>\n";
    commandHelp["help"] =               "fsc help <parameters : specific command (optional)> <flags : none>\n";
    commandHelp["create"] =             "fsc create <parameters : path> <flags : -f (specifies path is a file), -d (specifies path is a directory), -n (no automatic header guards in C and C++ related files)>\n";
    commandHelp["delete"] =             "fsc delete <parameters : path> <flags : -r (recursive), -s (silences confirmation prompt)>\n";
    commandHelp["list"] =               "fsc list <parameters : path> <flags : -r (recursive), -f (list files only), -d (list directories only)>\n";
    commandHelp["read"] =               "fsc read <parameters : path> <flags : none>\n";

    if (argumentParser->HasParameter(0))
    {
        if (!commandHelp.contains(argumentParser->GetParameter(0)))
        {
            throw std::runtime_error{ "\"" + argumentParser->GetParameter(0) + "\" is an invalid command. Use \"help\" to see a list of valid commands." };
        }
        std::cout << commandHelp[argumentParser->GetParameter(0)];
    }
    else
    {
        for (const auto& [_, info] : commandHelp)
        {
            std::cout << info << std::endl;
        }
    }
}

void CommandExecutor::Create()
{
    std::filesystem::path path{ argumentParser->GetParameter(0) };
    if (std::filesystem::exists(path))
    {
        throw std::runtime_error{ "Specified path already exists." };
    }
    path = std::filesystem::canonical(path);

    if (argumentParser->HasFlag("-f") && argumentParser->HasFlag("-d"))
    {
        throw std::runtime_error{ "Cannot use \"-f\" and \"-d\" flags at the same time. Use \"help create\" to see command structure." };
    }

    bool isFile{ false };
    if (path.has_extension())
    {
        if (!argumentParser->HasFlag("-d"))
        {
            isFile = true;
        }
    }
    else
    {
        if (argumentParser->HasFlag("-f"))
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
            throw std::runtime_error{ "Something went wrong creating \"" + path.string() + "\"." };
        }

        if (!argumentParser->HasFlag("-n"))
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

        std::cout << "Created file \"" + path.string() + "\"." << std::endl;
    }
    else
    {
        std::filesystem::create_directories(path);
        std::cout << "Created directory \"" + path.string() + "\"." << std::endl;
    }
}

void CommandExecutor::Delete()
{
    std::filesystem::path path{ argumentParser->GetParameter(0) };
    if (!std::filesystem::exists(path))
    {
        throw std::runtime_error{ "Specified path does not exist." };
    }
    path = std::filesystem::canonical(path);

    if (std::filesystem::is_directory(path))
    {
        if (std::filesystem::is_empty(path))
        {
            std::error_code error;
            std::filesystem::remove(path, error);
            if (error)
            {
                throw std::runtime_error{ "Something went wrong deleting directory \"" + path.string() + "\".\nError: " + error.message() };
            }
            std::cout << "Deleted directory \"" + path.string() + "\"." << std::endl;
        }
        else
        {
            if (!argumentParser->HasFlag("-r"))
            {
                throw std::runtime_error{ "Failed to delete \"" + path.string() + "\", Directory is not empty. Use \"-r\" (recursive) flag." };
            }

            if (!argumentParser->HasFlag("-s"))
            {
                if (!fsc_utilities::PromptConfirmation("Delete directory \"" + path.string() + "\"?"))
                {
                    return;
                }
            }

            std::error_code error;
            std::filesystem::remove_all(path, error);
            if (error)
            {
                throw std::runtime_error{ "Something went wrong deleting directory \"" + path.string() + "\".\nError: " + error.message() };
            }
            std::cout << "Deleted directory its contents \"" + path.string() + "\"." << std::endl;
        }
    }
    else
    {
        std::error_code error;
        std::filesystem::remove(path, error);
        if (error)
        {
            throw std::runtime_error{ "Something went wrong deleting file \"" + path.string() + "\".\nError: " + error.message() };
        }
        std::cout << "Deleted file \"" + path.string() + "\"." << std::endl;
    }
}

void CommandExecutor::List()
{
    std::filesystem::path path;
    if (argumentParser->HasParameter(0))
    {
        path = argumentParser->GetParameter(0);
    }
    else
    {
        path = std::filesystem::current_path();
    }
    if (!std::filesystem::exists(path))
    {
        throw std::runtime_error{ "Specified path does not exist." };
    }
    path = std::filesystem::canonical(path);

    if (!std::filesystem::is_directory(path))
    {
        throw std::runtime_error{ "Specified path is a file." };
    }

    bool filesOnly{ argumentParser->HasFlag("-f") };
    bool directoriesOnly{ argumentParser->HasFlag("-d") };
    if (filesOnly && directoriesOnly)
    {
        throw std::runtime_error{ "Cannot use \"-f\" and \"-d\" flags at the same time. Use \"help list\" to see command structure." };
    }

    try
    {
        if (argumentParser->HasFlag("-r"))
        {
            std::cout << "Listing \"" << path.string() + "\" recursively." << std::endl;
            for (const auto& entry : std::filesystem::recursive_directory_iterator(path))
            {
                if (std::filesystem::is_directory(entry))
                {
                    if (!filesOnly)
                    {
                        std::cout << "D: " + entry.path().filename().string() << std::endl;
                    }
                }
                else
                {
                    if (!directoriesOnly)
                    {
                        std::cout << "F: " + entry.path().filename().string() << std::endl;
                    }
                }
            }
        }
        else
        {
            std::cout << "Listing \"" << path.string() + "\"." << std::endl;
            for (const auto& entry : std::filesystem::directory_iterator(path))
            {
                if (std::filesystem::is_directory(entry))
                {
                    if (!filesOnly)
                    {
                        std::cout << "D: " + entry.path().filename().string() << std::endl;
                    }
                }
                else
                {
                    if (!directoriesOnly)
                    {
                        std::cout << "F: " + entry.path().filename().string() << std::endl;
                    }
                }
            }
        }
    }
    catch (const std::filesystem::filesystem_error& error)
    {
        throw std::runtime_error{ "Something went wrong listing directory \"" + path.string() + "\".\nError: " + error.what() };
    }
    
}

void CommandExecutor::Read()
{
    std::filesystem::path path{ argumentParser->GetParameter(0) };
    if (!std::filesystem::exists(path))
    {
        throw std::runtime_error{ "Specified path does not exist." };
    }
    path = std::filesystem::canonical(path);

    if (std::filesystem::is_directory(path))
    {
        throw std::runtime_error{ "Specified path is not a file." };
    }

    try
    {
        std::cout << fsc_utilities::ReadFile(path) << std::endl;
    }
    catch (const std::runtime_error& error)
    {
        throw error;
    }
}