#include "ExeCommand.h"
#include <iostream>
#include <ranges>
#if _WIN32
#include <process.h>
#else
#include <unistd.h>
#include <sys/wait.h>
#endif

ExeCommand::ExeCommand(const std::vector<std::string>& params) :
	Command(CommandType::EXE, params) 
{}

int ExeCommand::execute() const
{
	const char* program = m_params[0].c_str();
	std::vector<const char*> params;
	params.reserve(m_params.size() + 1);
	for (const auto& param : m_params)
	{
		params.push_back(param.c_str());
	}
	params.push_back(nullptr);
#if _WIN32
	return _execvp(program, params.data());
#else
	pid_t pid = fork();
	if (pid < 0)
	{
		std::cerr << "fork failed" << std::endl;
		return 1;
	}
	else if (pid == 0)
	{
		if (m_redirectOut)
		{
			dup2(m_fd[1], STDOUT_FILENO);
			close(m_fd[0]);
			close(m_fd[1]);
		}
		else if (m_redirectIn)
		{
			dup2(m_fd[0], STDIN_FILENO);
			close(m_fd[0]);
			close(m_fd[1]);
		}
		return execvp(program, const_cast<char* const*>(params.data()));
	}
	else
	{
		if (m_redirectIn)
		{
			close(m_fd[0]);
			close(m_fd[1]);
		}
		int status = 0;
		waitpid(pid, &status, 0);
		return WEXITSTATUS(status);
	}
#endif
}

void ExeCommand::redirectIn(int fd[2])
{
	m_redirectIn = true;
	m_fd[0] = fd[0];
	m_fd[1] = fd[1];
}

void ExeCommand::redirectOut(int fd[2])
{
	m_redirectOut = true;
	m_fd[0] = fd[0];
	m_fd[1] = fd[1];
}
