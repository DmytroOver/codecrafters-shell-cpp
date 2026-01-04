#include "UnknownCommand.h"
#include <iostream>

UnknownCommand::UnknownCommand(const std::vector<std::string>& params) :
	Command(CommandType::UNKNOWN, params)
{}

int UnknownCommand::execute() const
{
	std::cout << m_params[0] << ": command not found" << std::endl;
	return -1;
}