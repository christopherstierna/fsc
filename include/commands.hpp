#pragma once

#include "command_list.hpp"

namespace fsc
{
    void InitializeCommands() noexcept;
    const CommandList& GetCommandList() noexcept;
}