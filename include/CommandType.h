#pragma once
#include <string>
#include <unordered_map>

enum class CommandType : int
{
	EXIT,
	ECHO,
	TYPE,
	UNKNOWN
};

static const std::unordered_map<std::string, CommandType> s_commands = {
	{"exit", CommandType::EXIT},
	{"echo", CommandType::ECHO},
	{"type", CommandType::TYPE}
};
