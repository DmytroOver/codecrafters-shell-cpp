#pragma once
#include <ostream>
#include <string>

class OutputWriter
{
private:
	std::ostream& m_stdout;
	std::ostream& m_stderr;
public:
	OutputWriter() = delete;
	OutputWriter(OutputWriter& other) = delete;
	OutputWriter(std::ostream& _stdout, std::ostream& _stderr);
	void writeOutput(const std::string& str) const;
	void writeError(const std::string& str) const;
};

