
#include "ModbusSlave.h"

uint8_t ModbusSlave::m_NbSlaves = 0;

ModbusSlave::ModbusSlave()
{
    m_NbSlaves++;

    m_NameStr = "SLAVE_" + to_string(m_NbSlaves);

    std::cout << "created " << m_NameStr.c_str() << std::endl;

}


void ModbusSlave::Run()
{
     std::cout << "Starting Runner for " << m_NameStr.c_str() << std::endl;

}



