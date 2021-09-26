
#include "ModbusSlave.h"

uint8_t ModbusSlave::m_NbSlaves = 0U;

ModbusSlave::ModbusSlave(uint32_t tics_millisec, uint32_t sim_duration_seconds, SystemMonitor *sys_mon) : m_TicsMillisec(tics_millisec),
                                                                                                          m_SimDurationSeconds(sim_duration_seconds),
                                                                                                          m_sysMonPtr(sys_mon)
{
    m_NbSlaves++;

    m_NameStr = "SLAVE_" + to_string(m_NbSlaves);

    m_CurrentTic = 1U;
    m_LastTic = 1U;

    /* Initialize SLots for this ModbusSlave
     * The slow rate, for instance 60s will required to setup a slot at
     * an index in the range R = 60(s)/slot_duration(ms) = 60.1000/100 = 600
     * from (k + R) to (k + 2*R-1)
     * before k+R we refresh too early, beyond k+2R, data is not "fresh" enough
     * we missed a value
     **/
    m_NbSlots = ((uint32_t)m_sysMonPtr->GetRefreshRateSlow()) * 1000 * 2 / tics_millisec;

    cout << "Allocating " << m_NbSlots << " Slots" << endl;
    for (int i = 0; i < m_NbSlots; i++)
    {
        m_SlotsVector.push_back(Slot(i));
    }

    /*  for (int i = 0; i < m_NbSlots; i++)
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
        /* Update currentTic */
        this_slave->WaitTic();

        this_slave->LogCurSlot();

        this_slave->ProcessReplies();

        this_slave->ProcessRefresh();
    }

    return 0;
}

/*==================================================================================
 * PROCESS REPLIES
 * =================================================================================
 */
void ModbusSlave::ProcessReplies(void)
{
    uint16_t prev_slot = m_LastTic % m_NbSlots;

    /* Process replies, presumably to the Offsets from previous slot.*/
}

/*==================================================================================
 * ACTUAL SLOT PROCESSING
 * =================================================================================
 */
void ModbusSlave::ProcessRefresh(void)
{
    uint16_t cur_slot = m_CurrentTic % m_NbSlots;
    Slot *found = nullptr;
    /* This is a new tick, we might have missed a slot if we take longer to process responses
     * and setup futur requests, than allowed for the duration of a slot. In this case,
     * the CPU load mesurement will driver the # of request down. */

    /* Check for new offsets to manage */
    if (m_OffsetsToAdd.size())
    {
        for (size_t i = 0U; i < m_OffsetsToAdd.size(); i++)
        {
            /**/
            //  cout << "finding slot for new @" << m_OffsetsToAdd[i].m_OffsetValue << endl;
            found = FindSlot(m_OffsetsToAdd[i], cur_slot, true /*ASAP*/);

            if (nullptr == found)
            {
                cerr << "Could not find free slot to process " << m_OffsetsToAdd[i].dump().c_str() << " apply strategy so-and-so..." << endl;
                exit(0); // TODO
            }
        }

        m_OffsetsToAdd.clear();
    }

    /* Refresh Offsets from previous slot*/

    uint16_t prev_slot = (m_LastTic) % m_NbSlots;
    found = nullptr;

    if (m_SlotsVector[prev_slot].GetManagedOffsets())
    {
        for (size_t i = 0U; i < m_SlotsVector[prev_slot].m_OffsetsManaged.size(); i++)
        {
            /**/
            //  cout << "finding slot for new @" << m_OffsetsToAdd[i].m_OffsetValue << endl;
            found = FindSlot(m_SlotsVector[prev_slot].m_OffsetsManaged[i], cur_slot, false /*ASAP*/);

            if (nullptr == found)
            {
                cerr << "Could not find free slot to process " << m_SlotsVector[prev_slot].m_OffsetsManaged[i].dump().c_str() << " apply strategy so-and-so..." << endl;
                exit(0); // TODO
            }
        }

        m_SlotsVector[m_LastTic].ClearOffets();
    }

    /* Refresh Done */
    m_LastTic = m_CurrentTic;
}

/*==================================================================================
 * SLOT FINDING
 * =================================================================================
 */
Slot *ModbusSlave::FindSlot(const Offset &offset, uint16_t current_slot, bool insert_asap = false)
{
    uint16_t start_slot_offset = (offset.m_RefreshSeconds * 1000) / m_TicsMillisec; /*todo precompute in offset class*/
    uint16_t slot_span = 1000 / m_TicsMillisec;                                     /* over one second span */
    uint8_t min_requests = 99;
    Slot *suitable_slot = nullptr;
    bool forumId_already_managed = false;

    if (insert_asap)
    {
        /* start scanning from next slot onwards*/
        start_slot_offset = 0;
    }

    for (uint16_t i = start_slot_offset; i < start_slot_offset + slot_span; i++)
    {
        uint16_t K = (current_slot + i) % m_NbSlots;
        uint8_t requests = m_SlotsVector[K].GetManagedRequests();

        /* scan all possible slots, in case this forum id is already requested.
         * note that since */
        forumId_already_managed = m_SlotsVector[K].InManagedForumIds(offset.m_forumId);
        if (true == forumId_already_managed)
        {
            suitable_slot = &m_SlotsVector[K];
            break;
        }

        if ((requests < min_requests) && (requests < m_sysMonPtr->GetMaxRequestsPerSlot()))
        {
            min_requests = requests;
            suitable_slot = &m_SlotsVector[K];
        }

       /* if ((true == insert_asap) & (nullptr != suitable_slot))
        {
            break;
        }*/
    }

    if (nullptr != suitable_slot)
    {
        /**/
        suitable_slot->ManageOffset(offset, forumId_already_managed);
    }

    return suitable_slot;
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

void ModbusSlave::LogCurSlot()
{
    gLogger1.ColumnMode();
    gLogger1.Write(m_CurrentTic);

    uint16_t cur_slot = m_CurrentTic % m_NbSlots;

    gLogger1.LineMode();
    gLogger1.Write(cur_slot);

    gLogger1.Write(m_SlotsVector[cur_slot].GetManagedRequests());
    gLogger1.Write(m_SlotsVector[cur_slot].GetManagedOffsets());
}
