#pragma once
#include <string>

#include "Command.h"
class UnknownCommand :
    public Command
{
    std::string m_input;
public:
    UnknownCommand(const std::string& input);
    int execute() const override;
};

