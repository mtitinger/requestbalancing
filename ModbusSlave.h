
#include <cstdint>
#include <iostream>
#include <string>
#include "Logger.h"


using namespace std;

class ModbusSlave
{
    static uint8_t m_NbSlaves;

public:
     ModbusSlave();
    ~ModbusSlave(){};

public:

    void Run(uint32_t seconds_to_sim);

private:

    void Process();

    std::string m_NameStr;
};

