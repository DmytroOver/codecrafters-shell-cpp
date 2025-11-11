#pragma once
#include "CommandType.h"
#include <string>
#include <vector>

class Command
{
private:
	CommandType m_type;
protected:
	std::vector<std::string> m_params;
public:
	Command(CommandType type, const std::vector<std::string>& params);
	CommandType getType() const;
	virtual int execute() const = 0;
};

