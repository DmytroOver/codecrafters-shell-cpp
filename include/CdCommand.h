#pragma once
#include "Command.h"
class CdCommand :
    public Command
{
public:
    explicit CdCommand(const std::vector<std::string>& params);
    int execute() const override;
};

