#include "EchoCommand.h"
#include <iostream>
#include <ranges>

EchoCommand::EchoCommand(const std::vector<std::string>& params):
	Command(CommandType::ECHO, params)
{ }

int EchoCommand::execute() const
{
	std::string output = "";
	for (const auto& param : m_params | std::views::drop(1))
	{
		output += param + ' ';
	}
	std::cout << output << std::endl;
	return 0;
}