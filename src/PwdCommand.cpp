#include "PwdCommand.h"
#include "FileSystemHelper.h"

PwdCommand::PwdCommand(const std::vector<std::string>& params) :
	Command(CommandType::PWD, params) 
{}

int PwdCommand::execute() const
{
	writeOutput(FileSystemHelper::getInstance()->getPwd());
	return 0;
}
