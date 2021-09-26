

#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <pthread.h>
#include <chrono>
#include <thread>
#include <mutex>

/*CacheMonitor manager class*/
class SystemMonitor
{
private:
    /* data */
    uint8_t m_MaxRequestsPerSlot;
    uint8_t m_RefreshRateFast;
    uint8_t m_RefreshRateMedium;
    uint8_t m_RefreshRateSlow;

public:
    SystemMonitor(uint8_t max_requests_per_slot,
                  uint8_t refresh_rate_fast,
                  uint8_t refresh_rate_medium,
                  uint8_t refresh_rate_slow) : m_MaxRequestsPerSlot(max_requests_per_slot),
                                               m_RefreshRateFast(refresh_rate_fast),
                                               m_RefreshRateMedium(refresh_rate_medium),
                                               m_RefreshRateSlow(refresh_rate_slow)
    {
        m_curCPULoad = 50;
    };

    ~SystemMonitor() {};

    uint8_t m_curCPULoad; /*to simulate measured CPU load*/

public:
    uint8_t GetMaxRequestsPerSlot() { return m_MaxRequestsPerSlot; };
    uint8_t GetCurCPULoadPercent() { return m_curCPULoad; };
};