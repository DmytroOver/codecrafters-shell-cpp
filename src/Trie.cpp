#include "Trie.h"

Trie::Trie(): m_root()
{}

void Trie::insert(const std::string& word)
{
	TrieNode* currentNode = &m_root;
	for (char c : word)
	{
		currentNode->m_nodes.try_emplace(c, TrieNode());
		currentNode = &currentNode->m_nodes[c];
	}
	currentNode->isEnd = true;
}

bool Trie::search(const std::string& word) const
{
	const TrieNode* currentNode = &m_root;
	for (char c : word)
	{
		auto it = currentNode->m_nodes.find(c);
		if (it == currentNode->m_nodes.end())
		{
			return false;
		}
		currentNode = &it->second;
	}
	return currentNode->isEnd;
}

bool Trie::startsWith(const std::string& prefix) const
{
	const TrieNode* currentNode = &m_root;
	for (char c : prefix)
	{
		auto it = currentNode->m_nodes.find(c);
		if (it == currentNode->m_nodes.end())
		{
			return false;
		}
		currentNode = &it->second;
	}
	return true;
}

std::vector<std::string> Trie::autocomplete(const std::string& prefix) const
{
	std::vector<std::string> words;
	const TrieNode* currentNode = &m_root;
	for (char c : prefix)
	{
		auto it = currentNode->m_nodes.find(c);
		if (it == currentNode->m_nodes.end())
		{
			return words;
		}
		currentNode = &it->second;
	}
	dfs(*currentNode, prefix, words);

	return words;
}

void Trie::dfs(const TrieNode& node, const std::string& word, std::vector<std::string>& res) const
{
	if (node.isEnd)
	{
		res.push_back(word);
		if (node.m_nodes.empty())
		{
			return;
		}
	}

	for (const auto& pair : node.m_nodes)
	{
		dfs(pair.second, word + pair.first, res);
	}
}
