#include "PwdCommand.h"
#include "FileSystemHelper.h"

PwdCommand::PwdCommand(const std::vector<std::string>& params) :
	Command(CommandType::PWD, params) 
{}

int PwdCommand::execute() const
{
	writeString(FileSystemHelper::getInstance()->getPwd());
	return 0;
}
