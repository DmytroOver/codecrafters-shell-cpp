#pragma once
#include "CommandType.h"
#include <string>
#include <vector>

class Command
{
private:
	CommandType m_type;
	std::string m_outFilename;
protected:
	std::vector<std::string> m_params;
public:
	Command(CommandType type, const std::vector<std::string>& params);
	CommandType getType() const;
	void writeString(const std::string& str) const;
	virtual int execute() const = 0;
};

