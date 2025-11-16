#include "CdCommand.h"
#include "FileSystemHelper.h"

CdCommand::CdCommand(const std::vector<std::string>& params) :
	Command(CommandType::CD, params)
{}

int CdCommand::execute() const
{
	int errorCode = 0;
	std::string path = m_params.size() > 1 ? m_params[1] : "";
	if (path == "~")
	{
#ifdef _WIN32
		path = std::getenv("USERPROFILE");
#else
		path = std::getenv("HOME");
#endif
	}
	errorCode = FileSystemHelper::getInstance()->changePath(path);
	if (errorCode)
	{
		writeOutput("cd: " + path + ": No such file or directory");
	}
	return errorCode;
}
