#pragma once
#include "Command.h"
#include <string>

class EchoCommand :
    public Command
{
    std::string m_output;
public:
    EchoCommand(const std::string& output);
    int execute() const override;
};

