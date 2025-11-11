#define _CRT_SECURE_NO_WARNINGS
#include "TypeCommand.h"
#include <iostream>
#include <cstdlib>
#include <filesystem>
#include <ranges>
#include <sstream>

namespace fs = std::filesystem;

TypeCommand::TypeCommand(const std::vector<std::string>& params) :
	Command(CommandType::TYPE, params)
{
}

std::vector<std::string> TypeCommand::getPathDirs() const
{
	const std::string& pathEnv = std::getenv("PATH");
	std::vector<std::string> dirs;
	std::stringstream ss(pathEnv);
	std::string dir;
	char pathsep = fs::path::preferred_separator == '/' ? ':' : ';';
	while (std::getline(ss, dir, pathsep)) {
		if (!dir.empty() && fs::exists(dir))
		{
			dirs.push_back(dir);
		}
	}

	return dirs;
}

int TypeCommand::execute() const
{
	std::vector<std::string> dirs = getPathDirs();

	for (const auto& param : m_params | std::views::drop(1))
	{
		auto it = s_commands.find(param);
		if (it != s_commands.end())
		{
			std::cout << param << " is a shell builtin" << std::endl;
		}
		else
		{
			bool found = false;
			for (const auto& dir : dirs)
			{
				for (const auto& dirEntry : fs::directory_iterator{ dir })
				{
					const std::string& filename = dirEntry.path().filename().string();
					const std::string& stem = dirEntry.path().stem().string();
					if (param == filename || param == stem)
					{
						fs::perms perms = fs::status(dirEntry.path()).permissions();
						if ((perms & fs::perms::owner_exec) != fs::perms::none ||
							(perms & fs::perms::group_exec) != fs::perms::none ||
							(perms & fs::perms::others_exec) != fs::perms::none)
						{
							std::cout << param << " is " << dirEntry.path().string() << std::endl;
							found = true;
							break;
						}
					}
				}
				if (found)
				{
					break;
				}
			}
			if (!found)
			{
			std::cout << param << ": not found" << std::endl;
			}
		}
	}
	return 0;
}