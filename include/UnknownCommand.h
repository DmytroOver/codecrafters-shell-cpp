#pragma once

#include "Command.h"
class UnknownCommand :
    public Command
{
public:
    UnknownCommand(const std::vector<std::string>& params);
    int execute() const override;
};

