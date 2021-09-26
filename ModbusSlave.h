
#include <cstdint>
#include <iostream>
#include <string>
#include "Logger.h"
#include <pthread.h>
#include <chrono>
#include <thread>
#include <mutex>

using namespace std;
using namespace std::chrono;

class ModbusSlave
{
    static uint8_t m_NbSlaves;
    static void *CacheMonitor(void *tics);
    static void *EventSource(void *tics_millisec);

public:
    ModbusSlave(uint32_t tics_millisec, uint32_t sim_duration_seconds);
    ~ModbusSlave(){};

public:
    int Start(void);
    int Join(void);

    uint32_t GetMaxTics(void) { return m_SimDurationSeconds * 1000 / m_TicsMillisec - 1; };
    uint32_t GetTicsMillisec(void) { return m_TicsMillisec; };

    string GetName(void) { return m_NameStr; };

private:
    /*Wait for event source to unlock and unlock immediately*/
    void WaitTic(void)
    {
         while(m_LastSlot == m_CurrentSlot) {
        /* check for new work*/
        std::this_thread::sleep_for(milliseconds(20));
        }

        m_LastSlot = m_CurrentSlot;

        cout << m_NameStr.c_str() << " processing Slot " <<  m_LastSlot << endl;
    };

    /*do sleep for the duration of a tic, holding the mutex */
    void SleepTic(void)
    {
       // cout << "  SleepTic lock" << std::endl;
        std::this_thread::sleep_for(milliseconds(m_TicsMillisec));
      //  m_signal_mutex.lock();
            m_CurrentSlot++;
       // m_signal_mutex.unlock();
    };

    pthread_t m_CacheMonitorThread;
    pthread_t m_EventThread;

    std::string m_NameStr;

    uint32_t m_TicsMillisec;
    uint32_t m_SimDurationSeconds;

    uint16_t m_CurrentSlot;
    uint16_t m_LastSlot;

    std::mutex m_signal_mutex;
};
