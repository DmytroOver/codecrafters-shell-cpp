#include "ExitCommand.h"

ExitCommand::ExitCommand(int code) : 
	Command(CommandType::EXIT),
	m_code(code)
{}

int ExitCommand::execute() const
{
	return m_code;
}
