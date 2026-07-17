#pragma once
#include "Command.h"

class ExeCommand :
    public Command
{
    int m_fdIn[2] = {};
    int m_fdOut[2] = {};
    bool m_redirectOut = false;
    bool m_redirectIn = false;
public:
    ExeCommand(const std::vector<std::string>& params);
    int execute() const override;

    void redirectIn(int fd[2]) override;
    void redirectOut(int fd[2]) override;
};

