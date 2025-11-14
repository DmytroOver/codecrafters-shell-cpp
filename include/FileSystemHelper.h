#pragma once
#include <string>
#include <vector>

class FileSystemHelper
{
private:
	static FileSystemHelper* s_instance;
	FileSystemHelper();
	~FileSystemHelper();

	std::vector<std::string> m_pathDirs;

public:
	FileSystemHelper(FileSystemHelper& other) = delete;
	void operator=(const FileSystemHelper& other) = delete;

	static FileSystemHelper* getInstance();

	std::string findExePath(const std::string& name) const;
	std::string getPwd() const;
};

