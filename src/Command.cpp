#include "Command.h"

Command::Command(CommandType type): m_type(type)
{ }

CommandType Command::getType() const
{
	return m_type;
}