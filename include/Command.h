#pragma once
#include "CommandType.h"

class Command
{
	CommandType m_type;
public:
	Command(CommandType type);
	CommandType getType() const;
	virtual int execute() const = 0;
};

