#pragma once
#include "Command.h"
#include <string>
#include <vector>

class TypeCommand :
    public Command
{
public:
    TypeCommand(const std::vector<std::string>& params);
    int execute() const override;
};

