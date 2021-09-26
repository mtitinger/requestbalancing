
#include <cstdint>
#include <iostream>
#include <string>

using namespace std;

class ModbusSlave
{
    static uint8_t m_NbSlaves;

public:
     ModbusSlave();
    ~ModbusSlave(){};

public:

    void Run();

private:

    std::string m_NameStr;
};

