#pragma once
#include <functional>
#include <string>
#include <memory>
#include <unordered_map>

class Command;
enum class CommandType;

using CommandCreator = std::function<std::unique_ptr<Command>(const std::vector<std::string>&)>;
using CommandsMap = std::unordered_map <CommandType, CommandCreator>;

class CommandParser
{
	static const CommandsMap s_commandCreators;
public:
	std::unique_ptr<Command> getCommand(const std::string& input) const;
};

