#include <iostream>
#include "PwdCommand.h"
#include "FileSystemHelper.h"

PwdCommand::PwdCommand(const std::vector<std::string>& params) :
	Command(CommandType::PWD, params) 
{}

int PwdCommand::execute() const
{
	std::cout << FileSystemHelper::getInstance()->getPwd() << std::endl;
	return 0;
}
