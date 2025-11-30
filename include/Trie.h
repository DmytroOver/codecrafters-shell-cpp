#pragma once
#include <string>
#include <map>
#include <vector>

class Trie
{
	struct TrieNode
	{
		std::map<char, TrieNode> m_nodes;
		bool isEnd = false;
	} m_root;

public:
	Trie();
	void insert(const std::string& word);
	bool search(const std::string& word) const;
	bool startsWith(const std::string& prefix) const;
	std::vector<std::string> autocomplete(const std::string& prefix) const;

	void dfs(const TrieNode& node, const std::string& word, std::vector<std::string>& res) const;
};

