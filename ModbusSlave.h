
#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <pthread.h>
#include <chrono>
#include <thread>
#include <mutex>

#include "Logger.h"
#include "SystemMonitor.h"
#include "Slot.h"

using namespace std;
using namespace std::chrono;

class ModbusSlave
{
    static uint8_t m_NbSlaves;
    static void *CacheMonitor(void *tics);
    static void *EventSource(void *tics_millisec);

public:
    ModbusSlave(uint32_t tics_millisec, uint32_t sim_duration_seconds, SystemMonitor *sys_mon);
    ~ModbusSlave(){};

public:
    int Start(void);
    int Join(void);

    uint32_t GetMaxTics(void) { return m_SimDurationSeconds * 1000 / m_TicsMillisec - 1; };
    uint32_t GetTicsMillisec(void) { return m_TicsMillisec; };
    string GetName(void) { return m_NameStr; };

    uint32_t GetNbSlots(void) { return m_NbSlots; };

    void ProcessReplies(void);
    void ProcessRefresh(void);

    Slot *FindSlot(const Offset &offset, uint16_t current_slot, bool insert_asap);

    void AddOffsetToCache(fake_mapping_t *offset)
    {
        std::lock_guard<std::mutex> lk(m_offets_mutex);
        Offset tmp(offset);

        m_OffsetsToAdd.push_back(Offset(offset));

        cout << "Added to cache : @" << tmp.dump().c_str() << endl;
    }

private:
    /*Wait for event source to unlock and unlock immediately*/
    void WaitTic(void)
    {
        while (m_LastTic == m_CurrentTic)
        {
            /* check for new work*/
            std::this_thread::sleep_for(milliseconds(20));
        }

        m_LastTic = m_CurrentTic;
    };

    /*do sleep for the duration of a tic, holding the mutex */
    void SleepTic(void)
    {
        std::this_thread::sleep_for(milliseconds(m_TicsMillisec));
        m_CurrentTic++;
    };

    void LogCurSlot();

    pthread_t m_CacheMonitorThread;
    pthread_t m_EventThread;

    std::string m_NameStr;

    uint32_t m_TicsMillisec;
    uint32_t m_SimDurationSeconds;

    uint16_t m_LastTic;
    uint16_t m_CurrentTic; /* just a trick to ismulate an event source */
    uint16_t m_NbSlots;

    std::mutex m_offets_mutex;

    SystemMonitor *m_sysMonPtr;

    std::vector<Slot> m_SlotsVector;

    std::vector<Offset> m_OffsetsToAdd;
    std::vector<Offset> m_OffsetsToRemove;
};
