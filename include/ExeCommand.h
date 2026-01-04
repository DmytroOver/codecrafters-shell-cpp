#pragma once
#include "Command.h"

class ExeCommand :
    public Command
{
public:
    ExeCommand(const std::vector<std::string>& params);
    int execute() const override;
};

