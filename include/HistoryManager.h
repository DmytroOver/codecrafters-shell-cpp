#pragma once
#include <vector>
#include <string>

class HistoryManager
{
    std::vector<std::pair<int, std::string>> m_history;

    HistoryManager() = default;
public:
    HistoryManager(const HistoryManager&) = delete;
    HistoryManager& operator=(const HistoryManager&) = delete;

    static HistoryManager& getInstance();
    [[nodiscard]] const std::vector<std::pair<int, std::string>>& getHistory() const;
    void append(const std::string& input);
};
