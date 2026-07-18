#include "HistoryCommand.h"
#include <iostream>
#include <iomanip>
#include <ranges>
#include "HistoryManager.h"

HistoryCommand::HistoryCommand(const std::vector<std::string>& params) :
    Command(CommandType::UNKNOWN, params)
{}

int HistoryCommand::execute() const
{
    int n = m_params.size() > 1 ? std::atoi(m_params[1].c_str()) : 0;
    const auto& history = HistoryManager::getInstance().getHistory();
    int drop = n > 0 ? history.size() - n : 0;

    auto history_view = history | std::views::drop(std::max(0, drop));

    for (const auto& [index, input] : history_view)
    {
        std::cout << std::setw(5) << index << "  " << input << std::endl;
    }
    return 0;
}