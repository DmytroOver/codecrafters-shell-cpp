#pragma once
#include "CommandType.h"
#include <string>
#include <vector>
#include <memory>
#include <cstdio>

class Command
{
private:
	CommandType m_type;

	int m_stdout;
	int m_stderr;
	FILE* m_outfile = nullptr;
	FILE* m_errfile = nullptr;
protected:
	std::vector<std::string> m_params;
public:
	Command(CommandType type, const std::vector<std::string>& params);
	virtual ~Command();
	CommandType getType() const;
	virtual int execute() const = 0;
};

