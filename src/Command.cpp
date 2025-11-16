#include "Command.h"
#include "FileSystemHelper.h"
#include <iostream>
#include <fstream>

Command::Command(CommandType type, const std::vector<std::string>& params): 
	m_type(type)
{
	m_params.reserve(params.size());
	bool isFileOutput = false;
	for (const auto& param : params)
	{
		if (isFileOutput)
		{
			m_outFilename = param;
			break;
		}
		if (param == ">" || param == "1>")
		{
			isFileOutput = true;
			continue;
		}
		m_params.push_back(param);
	}
}

CommandType Command::getType() const
{
	return m_type;
}

void Command::writeString(const std::string& str) const
{
	if (m_outFilename.empty())
	{
		std::cout << str << std::endl;
		return;
	}

	FileSystemHelper::getInstance()->createDirs(m_outFilename);
	std::ofstream ofs(m_outFilename);
	ofs << str << std::endl;
}