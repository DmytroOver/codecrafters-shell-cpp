#pragma once
#include "CommandType.h"
#include "OutputWriter.h"
#include <string>
#include <vector>
#include <memory>

class Command
{
private:
	CommandType m_type;
	std::unique_ptr<OutputWriter> m_output;
protected:
	std::vector<std::string> m_params;
public:
	Command(CommandType type, const std::vector<std::string>& params);
	CommandType getType() const;
	void writeOutput(const std::string& str) const;
	void writeError(const std::string& str) const;
	virtual int execute() const = 0;
};

