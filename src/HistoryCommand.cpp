#include "HistoryCommand.h"
#include <iostream>
#include <iomanip>
#include "HistoryManager.h"

HistoryCommand::HistoryCommand(const std::vector<std::string>& params) :
    Command(CommandType::UNKNOWN, params)
{}

int HistoryCommand::execute() const
{
    const auto& history = HistoryManager::getInstance().getHistory();
    for (const auto& [index, input] : history)
    {
        std::cout << std::setw(5) << index << "  " << input << std::endl;
    }
    return 0;
}