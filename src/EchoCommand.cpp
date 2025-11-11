#include "EchoCommand.h"
#include <iostream>

EchoCommand::EchoCommand(const std::vector<std::string>& params):
	Command(CommandType::ECHO, params)
{ }

int EchoCommand::execute() const
{
	std::string output = "";
	bool skip = true;
	for (const auto& param : m_params)
	{
		if (skip)
		{
			skip = false;
			continue;
		}
		output += param + ' ';
	}
	std::cout << output << std::endl;
	return 0;
}