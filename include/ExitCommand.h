#pragma once
#include "Command.h"
class ExitCommand :
    public Command
{
public:
    ExitCommand(const std::vector<std::string>& params);
    int execute() const override;
};

