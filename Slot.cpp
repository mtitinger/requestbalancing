#include "Slot.h"

Slot::Slot(uint16_t index)
{
    m_SlotIndex = index;
}

Slot::~Slot()
{
}

bool Slot::InManagedForumIds(string ForumId)
{
    bool test = (std::find(m_ForumIdsManaged.begin(),
                           m_ForumIdsManaged.end(),
                           ForumId.c_str()) != m_ForumIdsManaged.end());

    if (test)
        std::cout << "FOUND: '" << ForumId << "' in slot " << m_SlotIndex << endl;
    
    return test;
}

void Slot::ManageOffset(Offset offset, bool forumId_already_managed)
{
    m_OffsetsManaged.push_back(offset);

    if (false == forumId_already_managed)
    {
        m_ForumIdsManaged.push_back(offset.m_forumId);
    }
}