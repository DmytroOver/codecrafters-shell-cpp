#pragma once

#include "Command.h"
class HistoryCommand :
    public Command
{
public:
    HistoryCommand(const std::vector<std::string>& params);
    int execute() const override;
};

