#include <iostream>
#include <string>
#include <vector>
#include <functional>

#include "commands.hpp"
#include "command_list.hpp"
#include "command_structure.hpp"
#include "command_functions.hpp"
#include "argument_parser.hpp"

namespace fsc
{
    CommandList commandList;

    void InitializeCommands() noexcept
    {
        commandList.AddCommandStructure(
            CommandStructure{
                "help",
                std::vector<Parameter>{
                    Parameter{ "command", ParameterRequirement::OPTIONAL, "Shows help for a specific command." },
                },
                std::vector<Flag>{},
                std::function<void(const ArgumentParser&)>{ Help }
            }
        );

        commandList.AddCommandStructure(
            CommandStructure{
                "create",
                std::vector<Parameter>{
                    Parameter{ "path", ParameterRequirement::REQUIRED, "Path to create." },
                },
                std::vector<Flag>{
                    Flag{ "-f", "Specifies that the path is a file, otherwise auto detect." },
                    Flag{ "-d", "Specifies that the path is a directory, otherwise auto detect." },
                    Flag{ "-n", "Prevents automatic header guards for C and C++ related files." }
                },
                std::function<void(const ArgumentParser&)>{ Create }
            }
        );

        commandList.AddCommandStructure(
            CommandStructure{
                "delete",
                std::vector<Parameter>{
                    Parameter{ "path", ParameterRequirement::REQUIRED, "Path to delete." },
                },
                std::vector<Flag>{
                    Flag{ "-r", "Recursively delete contents." },
                    Flag{ "-s", "Silence confirmation prompt." },
                    Flag{ "-c", "Delete contents only, requires \"-r\" flag." }
                },
                std::function<void(const ArgumentParser&)>{ Delete }
            }
        );

        commandList.AddCommandStructure(
            CommandStructure{
                "list",
                std::vector<Parameter>{
                    Parameter{ "path", ParameterRequirement::OPTIONAL, "Path to list." },
                },
                std::vector<Flag>{
                    Flag{ "-r", "List path recursively." },
                    Flag{ "-f", "list files only." },
                    Flag{ "-d", "List directories only." }
                },
                std::function<void(const ArgumentParser&)>{ List }
            }
        );

        commandList.AddCommandStructure(
            CommandStructure{
                "read",
                std::vector<Parameter>{
                    Parameter{ "path", ParameterRequirement::REQUIRED, "File to read." },
                },
                std::vector<Flag>{},
                std::function<void(const ArgumentParser&)>{ Read }          
            }
        );

        commandList.AddCommandStructure(
            CommandStructure{
                "clone",
                std::vector<Parameter>{
                    Parameter{ "target", ParameterRequirement::REQUIRED, "Target to clone." },
                    Parameter{ "destination", ParameterRequirement::REQUIRED, "Clone destination." }
                },
                std::vector<Flag>{
                    Flag{ "-o", "Overwrite existing item in destination if clone has the same name." },
                    Flag{ "-s", "Silence overwrite prompt." }
                },
                std::function<void(const ArgumentParser&)>{ Clone }          
            }
        );

        commandList.AddCommandStructure(
            CommandStructure{
                "move",
                std::vector<Parameter>{
                    Parameter{ "target", ParameterRequirement::REQUIRED, "Target to move." },
                    Parameter{ "destination", ParameterRequirement::REQUIRED, "Target destination." }
                },
                std::vector<Flag>{
                    Flag{ "-o", "Overwrite existing item in destination if target has the same name." },
                    Flag{ "-s", "Silence overwrite prompt." }
                },
                std::function<void(const ArgumentParser&)>{ Move }          
            }
        );

        commandList.AddCommandStructure(
            CommandStructure{
                "rename",
                std::vector<Parameter>{
                    Parameter{ "target", ParameterRequirement::REQUIRED, "Target to rename." },
                    Parameter{ "new name", ParameterRequirement::REQUIRED, "New name." }
                },
                std::vector<Flag>{
                    Flag{ "-o", "Overwrite existing item in destination if target has the same name." },
                    Flag{ "-s", "Silence overwrite prompt." }
                },
                std::function<void(const ArgumentParser&)>{ Rename }          
            }
        );

        commandList.AddCommandStructure(
            CommandStructure{
                "version",
                std::vector<Parameter>{},
                std::vector<Flag>{},
                std::function<void(const ArgumentParser&)>{ Version }          
            }
        );
    }

    const CommandList& GetCommandList() noexcept
    {
        return commandList;
    }
}