#include "UnknownCommand.h"
#include <iostream>

UnknownCommand::UnknownCommand(const std::vector<std::string>& params) :
	Command(CommandType::UNKNOWN, params)
{}

int UnknownCommand::execute() const
{
	writeString(m_params[0] + ": command not found");
	return -1;
}