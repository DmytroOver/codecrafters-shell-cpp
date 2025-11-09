#pragma once
#include "Command.h"
class ExitCommand :
    public Command
{
    int m_code;
public:
    ExitCommand(int code);
    int execute() const override;
};

