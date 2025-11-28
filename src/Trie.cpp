#include "Trie.h"

Trie::Trie(): m_root()
{}

void Trie::insert(const std::string & word)
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

std::string Trie::autocomplete(const std::string& prefix) const
{
	std::string word;
	const TrieNode* currentNode = &m_root;
	for (char c : prefix)
	{
		auto it = currentNode->m_nodes.find(c);
		if (it == currentNode->m_nodes.end())
		{
			return word;
		}
		currentNode = &it->second;
	}
	word = prefix;
	while (!currentNode->isEnd)
	{
		word += currentNode->m_nodes.cbegin()->first;
		currentNode = &currentNode->m_nodes.cbegin()->second;
	}

	return word;
}
