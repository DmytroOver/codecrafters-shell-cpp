#include "HistoryManager.h"

#include <fstream>

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
    m_currentIndex = m_history.size();
}

std::string_view HistoryManager::getPreviousCommand()
{
    if (m_history.empty())
    {
        return "";
    }

    m_currentIndex = std::max(0, m_currentIndex - 1);
    return m_history[m_currentIndex].second;
}

std::string_view HistoryManager::getNextCommand()
{
    m_currentIndex = std::min(static_cast<int>(m_history.size()), m_currentIndex + 1);
    if (m_currentIndex >= m_history.size())
    {
        return "";
    }

    return m_history[m_currentIndex].second;
}

void HistoryManager::readHistoryFromFile(const fs::path& filename)
{
    if (filename.empty())
    {
        return;
    }
    std::ifstream file{filename};
    if (!file.is_open())
    {
        return;
    }
    int index = !m_history.empty() ? m_history.back().first : 0;
    std::string line;
    while (std::getline(file, line) && !line.empty())
    {
        m_history.emplace_back(++index, line);
    }
}
