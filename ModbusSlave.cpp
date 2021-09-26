
#include "ModbusSlave.h"

uint8_t ModbusSlave::m_NbSlaves = 0;

ModbusSlave::ModbusSlave()
{
    m_NbSlaves++;

    m_NameStr = "SLAVE_" + to_string(m_NbSlaves);

    std::cout << "created " << m_NameStr.c_str() << std::endl;

}


void ModbusSlave::Process()
{
     std::cout << "*";
}

void ModbusSlave::Run(uint32_t seconds_to_sim)
{
     std::cout << "Running " << m_NameStr.c_str() << " for " << seconds_to_sim << " (sim) seconds" << std::endl;

     for (auto i=0; i< seconds_to_sim; i++)
     {
         Process();
     }
}



