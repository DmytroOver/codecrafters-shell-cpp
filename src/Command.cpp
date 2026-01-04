#include "Command.h"
#include "FileSystemHelper.h"
#include <iostream>
#include <fstream>
#if _WIN32
#include <io.h>
#endif

Command::Command(CommandType type, const std::vector<std::string>& params): 
	m_type(type)
{
#if _WIN32
	m_stdout = _dup(1);
	m_stderr = _dup(2);
#else
	m_stdout = dup(1);
	m_stderr = dup(2);
#endif

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
			m_outfile = std::fopen(param.c_str(), isAppendStdout ? "a" : "w");
#if _WIN32
			_dup2(_fileno(m_outfile), 1);
#else
			dup2(fileno(m_outfile), 1);
#endif
			break;
		}
		if (isStdErrRedirect)
		{
			FileSystemHelper::getInstance()->createDirs(param);
			m_errfile = std::fopen(param.c_str(), isAppendStderr ? "a" : "w");
#if _WIN32
			_dup2(_fileno(m_errfile), 1);
#else
			dup2(fileno(m_errfile), 1);
#endif
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
}

Command::~Command()
{
	if (m_outfile)
	{
#if _WIN32
		_dup2(m_stdout, 1);
#else
		dup2(m_stdout, STDOUT_FILENO);
#endif
		std::fclose(m_outfile);
		m_outfile = nullptr;
	}
	if (m_errfile)
	{
#if _WIN32
		_dup2(m_stderr, 1);
#else
		dup2(m_stderr, STDERR_FILENO);
#endif
		std::fclose(m_errfile);
		m_errfile = nullptr;
	}
}

CommandType Command::getType() const
{
	return m_type;
}
