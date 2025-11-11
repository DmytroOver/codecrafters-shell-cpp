#pragma once
#include "Command.h"

class EchoCommand :
    public Command
{
public:
    EchoCommand(const std::vector<std::string>& params);
    int execute() const override;
};

