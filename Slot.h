
#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include "Logger.h"
#include <pthread.h>
#include <chrono>
#include <thread>
#include <mutex>
#include <vector>

#include "Offset.h"

class Slot
{
private:
    /* The number of pending requests that will be emitted in this slot */
    uint8_t m_NbPendingRequests;
    uint16_t m_SlotIndex;

    std::vector<Offset> m_OffsetsManaged;
    std::vector<string> m_ForumIdsManaged;

public:
    Slot(uint16_t index);
    ~Slot();

    uint16_t GetSlotIndex(void) { return m_SlotIndex; };

    uint16_t GetManagedRequests(void) { return m_ForumIdsManaged.size(); };
    uint16_t GetManagedOffsets(void) { return m_OffsetsManaged.size(); };

    void ManageOffset(Offset offset, bool forumId_already_managed);

    void ClearOffets(void)
    {
        m_OffsetsManaged.clear();
        m_ForumIdsManaged.clear();
    };

    bool InManagedForumIds(string forum_id);
};
