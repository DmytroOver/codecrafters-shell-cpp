#include "CdCommand.h"
#include "FileSystemHelper.h"
#include <iostream>

CdCommand::CdCommand(const std::vector<std::string>& params) :
	Command(CommandType::CD, params)
{}

int CdCommand::execute() const
{
	int errorCode = 0;
	std::string path = m_params.size() > 1 ? m_params[1] : "";
	if (path == "~")
	{
		path = std::getenv("HOME");
	}
	errorCode = FileSystemHelper::getInstance()->changePath(path);
	if (errorCode)
	{
		std::cout << "cd: " << path << ": No such file or directory" << std::endl;
	}
	return errorCode;
}
