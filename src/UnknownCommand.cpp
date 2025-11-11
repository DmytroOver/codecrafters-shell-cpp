#include "UnknownCommand.h"
#include <iostream>

UnknownCommand::UnknownCommand(const std::vector<std::string>& params) :
	Command(CommandType::UNKNOWN, params)
{}

int UnknownCommand::execute() const
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
	std::cout << output << ": command not found" << std::endl;
	return -1;
}