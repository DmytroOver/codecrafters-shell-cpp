#include "CommandParser.h"
#include <vector>
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

CommandParser::CommandParser()
{
	for (const auto& pair : s_commands)
	{
		m_commandsTrie.insert(pair.first);
	}
}

std::vector<std::string> CommandParser::getTokens(const std::string& input) const
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

std::unique_ptr<Command> CommandParser::getCommand(const std::string& input) const
{
	CommandType command = CommandType::UNKNOWN;
	CommandCreator commandCreator = s_commandCreators.at(command);
	
	std::vector<std::string> tokens = getTokens(input);

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

std::string CommandParser::autocomplete(const std::string& prefix) const
{
	return m_commandsTrie.autocomplete(prefix);
}