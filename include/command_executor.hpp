#pragma once

#include "argument_parser.hpp"

class CommandExecutor
{
public:

    CommandExecutor(ArgumentParser& targetArgumentParser);

private:

    void Help();
    void Create();
    void Delete();
    void List();
    void Read();

    ArgumentParser* argumentParser;

};