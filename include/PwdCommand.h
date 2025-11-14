#pragma once
#include "Command.h"
class PwdCommand :
    public Command
{
public:
    PwdCommand(const std::vector<std::string>& params);
    int execute() const override;
};

