#include "EchoCommand.h"
#include <iostream>

EchoCommand::EchoCommand(const std::string& output):
	Command(CommandType::ECHO),
	m_output(output)
{ }

int EchoCommand::execute() const
{
	std::cout << m_output << std::endl;
	return 0;
}