#include "TypeCommand.h"
#include "FileSystemHelper.h"
#include <iostream>
#include <ranges>

TypeCommand::TypeCommand(const std::vector<std::string>& params) :
	Command(CommandType::TYPE, params)
{
}

int TypeCommand::execute() const
{
	for (const auto& param : m_params | std::views::drop(1))
	{
		auto it = s_commands.find(param);
		if (it != s_commands.end())
		{
			std::cout << param << " is a shell builtin" << std::endl;
			continue;
		}

		std::string path = FileSystemHelper::getInstance()->findExePath(param);
		if (!path.empty())
		{
			std::cout << param << " is " << path << std::endl;
		}
		else
		{
			std::cout << param << ": not found" << std::endl;
		}
	}
	return 0;
}