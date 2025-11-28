#pragma once
#include <string>
#include <vector>

#include <Trie.h>

class FileSystemHelper
{
private:
	static FileSystemHelper* s_instance;
	FileSystemHelper();
	~FileSystemHelper();

	std::vector<std::string> m_pathDirs;
	Trie m_filesTrie;

public:
	FileSystemHelper(FileSystemHelper&) = delete;
	FileSystemHelper(FileSystemHelper&&) = delete;
	FileSystemHelper& operator=(const FileSystemHelper&) = delete;
	FileSystemHelper& operator=(const FileSystemHelper&&) = delete;

	static FileSystemHelper* getInstance();

	std::string findExePath(const std::string& name) const;
	std::string getPwd() const;
	int changePath(const std::string& path) const;
	int createDirs(const std::string& path) const;
	const Trie& getFileTrie() const;
};

