#pragma once
#include "Command.h"
#include <string>
#include <vector>

class TypeCommand :
    public Command
{
private:
    std::vector<std::string> getPathDirs() const;
public:
    TypeCommand(const std::vector<std::string>& params);
    int execute() const override;
};

