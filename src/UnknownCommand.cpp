#include "UnknownCommand.h"
#include <iostream>

UnknownCommand::UnknownCommand(const std::string& input) : 
	Command(CommandType::UNKNOWN),
	m_input(input)
{}

int UnknownCommand::execute() const
{
	std::cout << m_input << ": command not found" << std::endl;
	return -1;
}