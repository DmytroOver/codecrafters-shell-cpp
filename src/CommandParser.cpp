#include "CommandParser.h"
#include <vector>
#include <sstream>
#include "FileSystemHelper.h"
#include "Command.h"
#include "ExitCommand.h"
#include "UnknownCommand.h"
#include "EchoCommand.h"
#include "TypeCommand.h"
#include "PwdCommand.h"
#include "CdCommand.h"

const CommandsMap CommandParser::s_commandCreators = {
	{CommandType::EXIT, [](const std::vector<std::string>& params) -> std::unique_ptr<Command> {return std::make_unique<ExitCommand>(params); }},
	{CommandType::ECHO, [](const std::vector<std::string>& params) -> std::unique_ptr<Command> {return std::make_unique<EchoCommand>(params); }},
	{CommandType::TYPE, [](const std::vector<std::string>& params) -> std::unique_ptr<Command> {return std::make_unique<TypeCommand>(params); }},
	{CommandType::PWD, [](const std::vector<std::string>& params) -> std::unique_ptr<Command> {return std::make_unique<PwdCommand>(params); }},
	{CommandType::CD, [](const std::vector<std::string>& params) -> std::unique_ptr<Command> {return std::make_unique<CdCommand>(params); }},
	{CommandType::UNKNOWN, [](const std::vector<std::string>& params) -> std::unique_ptr<Command> {return std::make_unique<UnknownCommand>(params); }}
};

std::unique_ptr<Command> CommandParser::getCommand(const std::string& input) const
{
	CommandType command = CommandType::UNKNOWN;
	std::vector<std::string> tokens;
	std::stringstream ss(input);
	std::string token;
	CommandCreator commandCreator = s_commandCreators.at(command);

	while (std::getline(ss, token, ' ')) {
		if (!token.empty())
		{
			tokens.push_back(token);
		}
	}

	if (tokens.empty())
	{
		return nullptr;
	}

	auto it = s_commands.find(tokens[0]);
	if (it != s_commands.end())
	{
		command = it->second;
	}

	if (command != CommandType::UNKNOWN)
	{
		commandCreator = s_commandCreators.at(command);
		return commandCreator(tokens);
	}

	std::string exePath = FileSystemHelper::getInstance()->findExePath(tokens[0]);
	if (!exePath.empty())
	{
		system(input.c_str());
		return nullptr;
	}

	return commandCreator(tokens);
}