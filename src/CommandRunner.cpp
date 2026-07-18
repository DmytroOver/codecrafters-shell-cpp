#include "CommandRunner.h"
#include <unistd.h>
#include <sys/wait.h>
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
#include "HistoryCommand.h"
#include "HistoryManager.h"

const CommandsMap CommandRunner::s_commandCreators = {
	{CommandType::EXIT, [](const std::vector<std::string>& params) {return std::make_unique<ExitCommand>(params); }},
	{CommandType::ECHO, [](const std::vector<std::string>& params) {return std::make_unique<EchoCommand>(params); }},
	{CommandType::TYPE, [](const std::vector<std::string>& params) {return std::make_unique<TypeCommand>(params); }},
	{CommandType::PWD, [](const std::vector<std::string>& params) {return std::make_unique<PwdCommand>(params); }},
	{CommandType::CD, [](const std::vector<std::string>& params) {return std::make_unique<CdCommand>(params); }},
	{CommandType::EXE, [](const std::vector<std::string>& params) {return std::make_unique<ExeCommand>(params); }},
	{CommandType::HISTORY, [](const std::vector<std::string>& params) {return std::make_unique<HistoryCommand>(params); }},
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

	int pipeCount = 0;
	std::vector<std::array<int, 2>> fds;
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
				fds.emplace_back();
				if (pipe(fds[pipeCount].data()) == -1)
				{
					std::cerr << "cannot create pipe \n";
					return;
				}
				commands.push_back(std::move(command));
				pipeCount++;
			}
			currentTokens.clear();
			continue;
		}
		else if (token.empty())
		{
			// end of input
			if (std::unique_ptr<Command> command = getCommand(currentTokens))
			{
				commands.push_back(std::move(command));
			}
			currentTokens.clear();
			continue;
		}
		currentTokens.push_back(token);
	}

	HistoryManager::getInstance().append(input);

	for (int i = 0; i < commands.size(); ++i)
	{
		if (pipeCount)
		{
			if (i < commands.size() - 1)
			{
				commands[i]->redirectOut(fds[i].data());
			}
			if (i > 0)
			{
				commands[i]->redirectIn(fds[i-1].data());
			}
		}
		commands[i]->execute();
		commands[i] = nullptr;
	}

	for (const auto& fd : fds)
	{
		close(fd[0]);
		close(fd[1]);
	}

	int status;
	while (wait(&status) != -1)
	{

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

std::string_view CommandRunner::getPreviousCommand() const
{
	return HistoryManager::getInstance().getPreviousCommand();
}

std::string_view CommandRunner::getNextCommand() const
{
	return HistoryManager::getInstance().getNextCommand();
}
