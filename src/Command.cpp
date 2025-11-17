#include "Command.h"
#include "FileSystemHelper.h"
#include <iostream>
#include <fstream>

Command::Command(CommandType type, const std::vector<std::string>& params): 
	m_type(type)
{
	m_params.reserve(params.size());
	bool isStdoutRedirect = false;
	bool isStdErrRedirect = false;
	std::ofstream stdoutfile;
	std::ofstream stderrfile;
	for (const auto& param : params)
	{
		if (isStdoutRedirect)
		{
			FileSystemHelper::getInstance()->createDirs(param);
			stdoutfile.open(param);
			break;
		}
		if (isStdErrRedirect)
		{
			FileSystemHelper::getInstance()->createDirs(param);
			stderrfile.open(param);
			break;
		}
		if (param == ">" || param == "1>")
		{
			isStdoutRedirect = true;
			continue;
		}
		if (param == "2>")
		{
			isStdErrRedirect = true;
			continue;
		}
		m_params.push_back(param);
	}
	m_output = std::make_unique<OutputWriter>(stdoutfile.is_open() ? stdoutfile : std::cout, 
		stderrfile.is_open() ? stderrfile : std::cout);
}

CommandType Command::getType() const
{
	return m_type;
}

void Command::writeOutput(const std::string& str) const
{
	m_output->writeOutput(str);
}

void Command::writeError(const std::string& str) const
{
	m_output->writeError(str);
}