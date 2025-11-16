#include "FileSystemHelper.h"
#include <cstdlib>
#include <filesystem>
#include <sstream>
#include <system_error>

namespace fs = std::filesystem;

FileSystemHelper* FileSystemHelper::s_instance;

FileSystemHelper::FileSystemHelper()
{
	const std::string& pathEnv = std::getenv("PATH");
	std::stringstream ss(pathEnv);
	std::string dir;
	char pathsep = fs::path::preferred_separator == '/' ? ':' : ';';
	while (std::getline(ss, dir, pathsep)) {
		if (!dir.empty() && fs::exists(dir))
		{
			m_pathDirs.push_back(dir);
		}
	}
}

FileSystemHelper::~FileSystemHelper() {}

FileSystemHelper* FileSystemHelper::getInstance()
{
	if (s_instance == nullptr)
	{
		s_instance = new FileSystemHelper();
	}
	return s_instance;
}

std::string FileSystemHelper::findExePath(const std::string& name) const
{
	for (const auto& dir : m_pathDirs)
	{
		for (const auto& dirEntry : fs::directory_iterator{ dir })
		{
			const std::string& filename = dirEntry.path().filename().string();
			const std::string& stem = dirEntry.path().stem().string();
			if (name == filename || name == stem)
			{
				fs::perms perms = fs::status(dirEntry.path()).permissions();
				if ((perms & fs::perms::owner_exec) != fs::perms::none ||
					(perms & fs::perms::group_exec) != fs::perms::none ||
					(perms & fs::perms::others_exec) != fs::perms::none)
				{
					return dirEntry.path().string();
				}
			}
		}
	}

	return "";
}

std::string FileSystemHelper::getPwd() const
{
	std::error_code error;
	fs::path path = fs::current_path(error);
	if (!error)
	{
		return path.string();
	}
	return "";
}

int FileSystemHelper::changePath(const std::string& path) const
{
	std::error_code error;
	fs::current_path(path, error);
	return error.value();
}

int FileSystemHelper::createDirs(const std::string& path) const
{
	fs::path fspath{ path };
	if (!fs::is_directory(fspath))
	{
		fspath = fspath.remove_filename();
	}
	if (fs::exists(fspath))
	{
		return 0;
	}
	std::error_code error;
	fs::create_directories(fspath, error);
	return error.value();

}
