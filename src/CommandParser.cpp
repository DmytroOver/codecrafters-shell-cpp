#include "CommandParser.h"
#include <vector>
#include <sstream>
#include "Command.h"
#include "ExitCommand.h"
#include "UnknownCommand.h"
#include "EchoCommand.h"

Command* CommandParser::getCommand(const std::string& input) const
{
	std::vector<std::string> tokens;
	std::stringstream ss(input);
	std::string token;

	while (std::getline(ss, token, ' ')) {
		tokens.push_back(token);
	}

	if (tokens.empty())
	{
		return new UnknownCommand(input);
	}

	const std::string& commandStr = tokens[0];
	if (commandStr == "exit")
	{
		int code = tokens.size() > 1 ? std::atoi(tokens[1].c_str()) : 0;
		return new ExitCommand(code);
	}
	else if (commandStr == "echo")
	{
		std::string output = "";
		for (int i = 1; i < tokens.size(); i++)
		{
			if (!tokens[i].empty())
			{
				output += tokens[i] + ' ';
			}
		}
		output[output.size() - 1] = '\0';
		return new EchoCommand(output);
	}

	return new UnknownCommand(input);
}