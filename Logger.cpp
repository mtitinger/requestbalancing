#include "Logger.h"

Logger::Logger(const char *filename) : m_Opt(LOGGER_COLUMN)
{
    string tmp = string(filename) + ".log";
    m_Ofs.open(tmp, std::ios_base::out | std::ios_base::trunc);
}

void Logger::Write(std::string strVal)
{
    return Write(strVal.c_str());
}

void Logger::Write(uint32_t val)
{
    return Write(std::to_string(val));
}

void Logger::Write(const char *strVal)
{
    if (nullptr != strVal)
    {
        if (m_Opt & LOGGER_COLUMN)
        {
            m_Ofs << std::endl;
        }
        else
        {
            m_Ofs << '\t';
        }

        m_Ofs << strVal;
    }
}

void Logger::Test()
{
    Write("Initializing Logger");

    string aa = "Testing Integer logging:";
    Write(aa);

    LineMode();

    uint32_t bb = 123456;
    Write(bb);

    ColumnMode();

    Write("Done");
}
