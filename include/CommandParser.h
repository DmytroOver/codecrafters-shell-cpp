#pragma once
#include <string>

class Command;

class CommandParser
{
public:
	Command* getCommand(const std::string& input) const;
};

