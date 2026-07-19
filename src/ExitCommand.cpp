#include "ExitCommand.h"
#include "HistoryManager.h"

ExitCommand::ExitCommand(const std::vector<std::string>& params) :
	Command(CommandType::EXIT, params)
{}

int ExitCommand::execute() const
{
	int code = 0;
	if (m_params.size() > 1)
	{
		code = std::atoi(m_params[1].c_str());
	}
	const char* historyFile = std::getenv("HISTFILE");
	if (historyFile)
	{
		HistoryManager::getInstance().writeHistoryToFile(historyFile);
	}
	exit(code);
}
