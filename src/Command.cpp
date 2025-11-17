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
	bool isAppendStdout = false;
	bool isAppendStderr = false;
	
	for (const auto& param : params)
	{
		if (isStdoutRedirect)
		{
			FileSystemHelper::getInstance()->createDirs(param);
			m_stdoutfile.open(param, isAppendStdout ? std::ios::app : std::ios::out);
			break;
		}
		if (isStdErrRedirect)
		{
			FileSystemHelper::getInstance()->createDirs(param);
			m_stderrfile.open(param, isAppendStderr ? std::ios::app : std::ios::out);
			break;
		}
		if (param == ">" || param == "1>")
		{
			isStdoutRedirect = true;
			continue;
		}
		if (param == ">>" || param == "1>>")
		{
			isStdoutRedirect = true;
			isAppendStdout = true;
			continue;
		}
		if (param == "2>")
		{
			isStdErrRedirect = true;
			continue;
		}
		if (param == "2>>")
		{
			isStdErrRedirect = true;
			isAppendStderr = true;
			continue;
		}
		m_params.push_back(param);
	}
	m_output = std::make_unique<OutputWriter>(m_stdoutfile.is_open() ? m_stdoutfile : std::cout, 
		m_stderrfile.is_open() ? m_stderrfile : std::cout);
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