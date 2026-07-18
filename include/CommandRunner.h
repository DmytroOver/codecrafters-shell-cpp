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

class CommandRunner
{
private:
	static const CommandsMap s_commandCreators;
	Trie m_commandsTrie;

	std::vector<std::string> getTokens(const std::string& input) const;
public:
	CommandRunner();
	CommandRunner(CommandRunner&) = delete;
	CommandRunner(CommandRunner&&) = delete;
	CommandRunner& operator=(const CommandRunner&) = delete;
	CommandRunner& operator=(CommandRunner&&) = delete;
	~CommandRunner() = default;
	void run(const std::string& input) const;
	[[nodiscard]] std::unique_ptr<Command> getCommand(const std::vector<std::string>& input) const;
	[[nodiscard]] std::vector<std::string> autocomplete(const std::string& prefix) const;
	[[nodiscard]] std::string_view getPreviousCommand() const;
	[[nodiscard]] std::string_view getNextCommand() const;
};

