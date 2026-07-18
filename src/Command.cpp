#include "Command.h"
#include "FileSystemHelper.h"
#include <fstream>
#include <iostream>
#if _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

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
			m_outfile = std::fopen(param.c_str(), isAppendStdout ? "a" : "w");
#if _WIN32
			_dup2(_fileno(m_outfile), 1);
#else
			m_stdout = dup(STDOUT_FILENO);
			dup2(fileno(m_outfile), STDOUT_FILENO);
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
			m_stderr = dup(STDERR_FILENO);
			dup2(fileno(m_errfile), STDERR_FILENO);
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
	if (m_stdin > 0)
	{
		dup2(m_stdin, STDIN_FILENO);
		close(m_stdin);
	}
	if (m_stdout > 0)
	{
		dup2(m_stdout, STDOUT_FILENO);
		close(m_stdout);
	}
	if (m_stderr > 0)
	{
		dup2(m_stderr, STDERR_FILENO);
		close(m_stderr);
	}
	if (m_outfile)
	{
		std::fclose(m_outfile);
		m_outfile = nullptr;
	}
	if (m_errfile)
	{
		std::fclose(m_errfile);
		m_errfile = nullptr;
	}
}

CommandType Command::getType() const
{
	return m_type;
}

void Command::redirectOut(int fd[2])
{
	m_stdout = dup(STDOUT_FILENO);
	if (fd[1] == -1)
	{
		return;
	}
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
}

void Command::redirectIn(int fd[2])
{
	m_stdin = dup(STDIN_FILENO);
	if (fd[0] == -1)
	{
		return;
	}
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
}
