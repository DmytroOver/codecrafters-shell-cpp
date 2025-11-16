#include "TypeCommand.h"
#include "FileSystemHelper.h"
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
			writeOutput(param + " is a shell builtin");
			continue;
		}

		std::string path = FileSystemHelper::getInstance()->findExePath(param);
		if (!path.empty())
		{
			writeOutput(param + " is " + path);
		}
		else
		{
			writeOutput(param + ": not found");
		}
	}
	return 0;
}