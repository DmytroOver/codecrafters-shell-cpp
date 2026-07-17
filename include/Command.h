#pragma once
#include "CommandType.h"
#include <string>
#include <vector>

class Command
{
	CommandType m_type;

	int m_stdin = -1;
	int m_stdout = -1;
	int m_stderr = -1;
	FILE* m_outfile = nullptr;
	FILE* m_errfile = nullptr;
protected:
	std::vector<std::string> m_params;
public:
	Command(CommandType type, const std::vector<std::string>& params);
	virtual ~Command();
	CommandType getType() const;
	virtual void redirectOut(int fd[2]);
	virtual void redirectIn(int fd[2]);
	virtual int execute() const = 0;
};

