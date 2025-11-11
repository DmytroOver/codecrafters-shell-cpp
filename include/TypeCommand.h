#pragma once
#include "Command.h"
class TypeCommand :
    public Command
{
public:
    TypeCommand(const std::vector<std::string>& params);
    int execute() const override;
};

