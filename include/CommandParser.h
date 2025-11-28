#pragma once
#include <functional>
#include <string>
#include <memory>
#include <unordered_map>
#include "Trie.h"

class Command;
enum class CommandType;

using CommandCreator = std::function<std::unique_ptr<Command>(const std::vector<std::string>&)>;
using CommandsMap = std::unordered_map <CommandType, CommandCreator>;

class CommandParser
{
private:
	static const CommandsMap s_commandCreators;
	Trie m_commandsTrie;

	std::vector<std::string> getTokens(const std::string& input) const;
public:
	CommandParser();
	CommandParser(CommandParser&) = delete;
	CommandParser(CommandParser&&) = delete;
	CommandParser& operator=(const CommandParser&) = delete;
	CommandParser& operator=(CommandParser&&) = delete;
	~CommandParser() = default;
	std::unique_ptr<Command> getCommand(const std::string& input) const;
	std::string autocomplete(const std::string& prefix) const;
};

