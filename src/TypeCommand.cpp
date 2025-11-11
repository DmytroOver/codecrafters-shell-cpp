#include "TypeCommand.h"
#include <iostream>

TypeCommand::TypeCommand(const std::vector<std::string>& params) :
	Command(CommandType::TYPE, params)
{
}

int TypeCommand::execute() const
{
	std::string command = m_params.size() > 1 ? m_params[1] : "";
	auto it = s_commands.find(command);
	if (it != s_commands.end())
	{
		std::cout << command << " is a shell builtin" << std::endl;
	}
	else
	{
		std::cout << command << ": not found" << std::endl;
	}
	return 0;
}