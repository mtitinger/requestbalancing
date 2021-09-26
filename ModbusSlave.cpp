
#include "ModbusSlave.h"

uint8_t ModbusSlave::m_NbSlaves = 0;

ModbusSlave::ModbusSlave(uint32_t tics_millisec, uint32_t sim_duration_seconds, SystemMonitor *sys_mon) : m_TicsMillisec(tics_millisec),
                                                                                                          m_SimDurationSeconds(sim_duration_seconds),
                                                                                                          m_sysMonPtr(sys_mon)
{
    m_NbSlaves++;

    m_NameStr = "SLAVE_" + to_string(m_NbSlaves);

    m_CurrentTic = 0;
    m_LastTic = 0;

    /* Initialize SLots for this ModbusSlave
     * The slow rate, for instance 60s will required to setup a slot at
     * an index in the range R = 60(s)/slot_duration(ms) = 60.1000/100 = 600
     * from (k + R) to (k + 2*R-1)
     * before k+R we refresh too early, beyond k+2R, data is not "fresh" enough
     * we missed a value
     **/
    uint16_t slots_to_allocate = m_sysMonPtr->GetefreshRateSlow() * 1000 * 2 / tics_millisec;

    cout << "Allocating " << slots_to_allocate << " Slots" << endl;
    for (int i = 0; i < slots_to_allocate; i++)
    {
        m_SlotsVector.push_back(Slot(i));
    }

  /*  for (int i = 0; i < slots_to_allocate; i++)
    {
        out << "Allocated Slot " << m_SlotsVector[i].GetSlotIndex() << " at " << i << endl;
    }*/
}

/*==================================================================================
 * will be joined, will stop when max tics to sim is reached
 * =================================================================================
 */
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

        this_slave->ProcessSlot();
    }

    return 0;
}

/*==================================================================================
 * ACTUAL SLOT PROCESSING
 * =================================================================================
 */
void ModbusSlave::ProcessSlot()
{
    /* This is a new tick, we might have missed a slot if we take longer to process responses
     * and setup futur requests, than allowed for the duration of a slot. In this case,
     * the CPU load mesurement will driver the # of request down. */

    /* Check for pending replies */


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
