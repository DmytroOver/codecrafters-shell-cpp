#include "OutputWriter.h"

OutputWriter::OutputWriter(std::ostream& _stdout, std::ostream& _stderr):
	m_stdout(_stdout),
	m_stderr(_stderr)
{ }

void OutputWriter::writeOutput(const std::string& str) const
{
	m_stdout << str << std::endl;
}

void OutputWriter::writeError(const std::string& str) const
{
	m_stderr << str << std::endl;
}
