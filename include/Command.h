#pragma once
#include "CommandType.h"
#include <string>
#include <vector>

enum class RedirectType
{
	STDOUT,
	STDERR
};

class Command
{
private:
	CommandType m_type;
	std::string m_outFilename;
	RedirectType m_redirectType;
protected:
	std::vector<std::string> m_params;
public:
	Command(CommandType type, const std::vector<std::string>& params);
	CommandType getType() const;
	void writeOutput(const std::string& str) const;
	void writeError(const std::string& str) const;
	virtual int execute() const = 0;
};

