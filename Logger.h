#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

#define LOGGER_COLUMN 0x1

class Logger
{
    uint32_t m_Opt;
    ofstream m_Ofs;

public:
    Logger(const char *filename);
    ~Logger() { m_Ofs.close(); };

    void Write(std::string);
    void Write(uint32_t val);
    void Write(const char *strVal);

    void SetOpt(uint32_t opt) { m_Opt = opt; };
    uint32_t GetOpt(void) { return m_Opt; };

    void LineMode() { m_Opt &= ~LOGGER_COLUMN; };
    void ColumnMode() { m_Opt |= LOGGER_COLUMN; };

    void Test();
};
