#pragma once

#include "argument_parser.hpp"

namespace fsc
{
    void Help(const ArgumentParser& argumentParser);
    void Create(const ArgumentParser& argumentParser);
    void Delete(const ArgumentParser& argumentParser);
    void List(const ArgumentParser& argumentParser);
    void Read(const ArgumentParser& argumentParser);
}