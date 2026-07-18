#include "HistoryManager.h"

HistoryManager& HistoryManager::getInstance()
{
    static HistoryManager instance;
    return instance;
}

const std::vector<std::pair<int, std::string>>& HistoryManager::getHistory() const
{
    return m_history;
}

void HistoryManager::append(const std::string& input)
{
    const int index = !m_history.empty() ? m_history.back().first : 0;
    m_history.emplace_back(index + 1, input);
}
