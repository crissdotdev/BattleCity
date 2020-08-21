#include "Logger.h"



Logger::Logger(std::ostream& out, Level minLogLevel) :m_out(out), m_minLogLevel(minLogLevel)
{
}

Logger::~Logger()
{
}