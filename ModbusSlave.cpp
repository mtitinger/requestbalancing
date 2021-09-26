
#include "ModbusSlave.h"

uint8_t ModbusSlave::m_NbSlaves = 0;

ModbusSlave::ModbusSlave(uint32_t tics_millisec, uint32_t sim_duration_seconds, SystemMonitor *sys_mon) : m_TicsMillisec(tics_millisec),
                                                                                                          m_SimDurationSeconds(sim_duration_seconds),
                                                                                                          m_sysMonPtr(sys_mon)
{
    m_NbSlaves++;

    m_NameStr = "SLAVE_" + to_string(m_NbSlaves);

    m_CurrentSlot = 0;
    m_LastSlot = 0;

    /*Initialize SLots for this ModbusSlave*/
}

/* will be joined, will stop when max tics to sim is reached */
void *ModbusSlave::CacheMonitor(void *slave)
{
    ModbusSlave *this_slave = (ModbusSlave *)slave;
    uint32_t tics_to_go = this_slave->GetMaxTics();

    cout << "- Starting CacheMonitor for " << this_slave->GetName().c_str() << std::endl;
    cout << "  running for " << tics_to_go << " tics" << std::endl;

    /*Wait for event source to unlock and unlock immediately*/
    while (tics_to_go--)
    {
        this_slave->WaitTic();
        std::cout << tics_to_go << std::endl;
    }

    return 0;
}

/* Will not be waited upon, no join, will be canceled at end of sim */
void *ModbusSlave::EventSource(void *slave)
{
    ModbusSlave *this_slave = (ModbusSlave *)slave;

    std::cout << "- starting event source for " << this_slave->GetName().c_str() << std::endl;
    std::cout << "  with tics of " << this_slave->GetTicsMillisec() << " ms" << std::endl;

    while (1)
    {
        this_slave->SleepTic();
    }

    return 0;
}

int ModbusSlave::Start(void)
{
    int ret = 0;

    std::cout << "Starting " << m_NameStr.c_str() << std::endl;

    ret = pthread_create(&m_EventThread, NULL, ModbusSlave::EventSource, (void *)this);

    // std::this_thread::sleep_for(milliseconds(1000));
    ret = pthread_create(&m_CacheMonitorThread, NULL, ModbusSlave::CacheMonitor, (void *)this);

    return ret;
}

int ModbusSlave::Join(void)
{
    pthread_join(m_CacheMonitorThread, NULL);
    cout << m_NameStr.c_str() << "Sim done" << endl;
    return 0;
}
