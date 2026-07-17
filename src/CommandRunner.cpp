#include "CommandRunner.h"
#include <unistd.h>
#include <iostream>
#include <vector>
#include "FileSystemHelper.h"
#include "Command.h"
#include "ExitCommand.h"
#include "UnknownCommand.h"
#include "EchoCommand.h"
#include "TypeCommand.h"
#include "PwdCommand.h"
#include "CdCommand.h"
#include "ExeCommand.h"

const CommandsMap CommandRunner::s_commandCreators = {
	{CommandType::EXIT, [](const std::vector<std::string>& params) {return std::make_unique<ExitCommand>(params); }},
	{CommandType::ECHO, [](const std::vector<std::string>& params) {return std::make_unique<EchoCommand>(params); }},
	{CommandType::TYPE, [](const std::vector<std::string>& params) {return std::make_unique<TypeCommand>(params); }},
	{CommandType::PWD, [](const std::vector<std::string>& params) {return std::make_unique<PwdCommand>(params); }},
	{CommandType::CD, [](const std::vector<std::string>& params) {return std::make_unique<CdCommand>(params); }},
	{CommandType::EXE, [](const std::vector<std::string>& params) {return std::make_unique<ExeCommand>(params); }},
	{CommandType::UNKNOWN, [](const std::vector<std::string>& params) {return std::make_unique<UnknownCommand>(params); }}
};

CommandRunner::CommandRunner()
{
	for (const auto& pair : s_commands)
	{
		m_commandsTrie.insert(pair.first);
	}
}

std::vector<std::string> CommandRunner::getTokens(const std::string& input) const
{
	std::vector<std::string> tokens;
	std::string token;
	token.reserve(input.size());
	bool singleQuoteOpen = false;
	bool doubleQuoteOpen = false;
	bool escapeChar = false;
	for (char c : input)
	{
		if (doubleQuoteOpen && escapeChar && c != '"' && c != '\\')
		{
			token.push_back('\\');
		}
		switch (c)
		{
		case ' ':
		{
			if (singleQuoteOpen || doubleQuoteOpen || escapeChar)
			{
				token.push_back(c);
				escapeChar = false;
			}
			else if (!token.empty())
			{
				tokens.push_back(token);
				token.clear();
			}
			break;
		}
		case '\'':
		{
			if (doubleQuoteOpen || escapeChar)
			{
				token.push_back(c);
				escapeChar = false;
				break;
			}
			singleQuoteOpen = !singleQuoteOpen;
			break;
		}
		case '"':
		{
			if (singleQuoteOpen || escapeChar)
			{
				token.push_back(c);
				escapeChar = false;
				break;
			}
			doubleQuoteOpen = !doubleQuoteOpen;
			break;
		}
		case '\\':
		{
			if (singleQuoteOpen || escapeChar)
			{
				token.push_back(c);
				escapeChar = false;
				break;
			}
			escapeChar = true;
			break;
		}
		default:
		{
			token.push_back(c);
			escapeChar = false;
			break;
		}
		}
	}

	if (!token.empty())
	{
		tokens.push_back(token);
	}

	return tokens;
}

void CommandRunner::run(const std::string& input) const
{
	std::vector<std::string> tokens = getTokens(input);

	if (tokens.empty())
	{
		return;
	}
	tokens.emplace_back("");

	std::vector<std::unique_ptr<Command>> commands;
	std::vector<std::string> currentTokens;
	currentTokens.reserve(tokens.size());

	bool isPipe = false;
	int fd[2];
	for (const auto& token : tokens)
	{
		if (token == "|")
		{
			if (currentTokens.empty())
			{
				std::cerr << "parse error near '" << token << "'\n";
				return;
			}
			if (std::unique_ptr<Command> command = getCommand(currentTokens))
			{
				if (pipe(fd) < 0)
				{
					std::cerr << "cannot create pipe \n";
					return;
				}
				command->redirectOut(fd);
				commands.push_back(std::move(command));
				isPipe = true;
			}
			currentTokens.clear();
			continue;
		}
		else if (token.empty())
		{
			// end of input
			if (std::unique_ptr<Command> command = getCommand(currentTokens))
			{
				if (isPipe)
				{
					command->redirectIn(fd);
				}
				commands.push_back(std::move(command));
			}
			currentTokens.clear();
			continue;
		}
		currentTokens.push_back(token);
	}

	for (auto& cmd : commands)
	{
		cmd->execute();
		cmd = nullptr;
	}
}

std::unique_ptr<Command> CommandRunner::getCommand(const std::vector<std::string>& tokens) const
{
	CommandType command = CommandType::UNKNOWN;
	CommandCreator commandCreator = s_commandCreators.at(command);

	if (tokens.empty())
	{
		return nullptr;
	}

	auto it = s_commands.find(tokens[0]);
	if (it != s_commands.end())
	{
		command = it->second;
	}

	// if command is built-in
	if (command != CommandType::UNKNOWN)
	{
		commandCreator = s_commandCreators.at(command);
	}
	else // maybe executable
	{
		std::string exePath = FileSystemHelper::getInstance()->findExePath(tokens[0]);
		if (!exePath.empty())
		{
			commandCreator = s_commandCreators.at(CommandType::EXE);
		}
	}

	return commandCreator(tokens);
}

std::vector<std::string> CommandRunner::autocomplete(const std::string& prefix) const
{
	std::vector<std::string> result = m_commandsTrie.autocomplete(prefix);
	if (result.empty())
	{
		result = FileSystemHelper::getInstance()->getFileTrie().autocomplete(prefix);
	}
	return result;
}