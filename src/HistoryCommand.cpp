#include "HistoryCommand.h"
#include <iostream>

HistoryCommand::HistoryCommand(const std::vector<std::string>& params) :
    Command(CommandType::UNKNOWN, params)
{}

int HistoryCommand::execute() const
{
    std::cout << "history not implemented" << std::endl;
    return 0;
}