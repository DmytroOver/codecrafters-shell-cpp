#include "Command.h"

Command::Command(CommandType type, const std::vector<std::string>& params): 
	m_type(type),
	m_params(params)
{ }

CommandType Command::getType() const
{
	return m_type;
}