#pragma once
#include <vector>
#include <string>

class HistoryManager
{
    std::vector<std::pair<int, std::string>> m_history;
    int m_currentIndex = 0;

    HistoryManager() = default;
public:
    HistoryManager(const HistoryManager&) = delete;
    HistoryManager& operator=(const HistoryManager&) = delete;

    static HistoryManager& getInstance();
    [[nodiscard]] const std::vector<std::pair<int, std::string>>& getHistory() const;
    void append(const std::string& input);
    [[nodiscard]] std::string_view getPreviousCommand();
    [[nodiscard]] std::string_view getNextCommand();
};
