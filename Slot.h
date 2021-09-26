
#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include "Logger.h"
#include <pthread.h>
#include <chrono>
#include <thread>
#include <mutex>

//#include "Offets.h"


class Slot
{
    static uint16_t NbSlots;

private:
    /* The number of pending requests that will be emitted in this slot */
    uint8_t     m_NbPendingRequests;
    uint16_t    m_SlotIndex;

public:
    Slot(/* args */);
    ~Slot();

    uint16_t GetSlotIndex(void) { return m_SlotIndex; };
};
