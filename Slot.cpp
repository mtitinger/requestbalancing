#include "Slot.h"

uint16_t Slot::NbSlots = 0U;

Slot::Slot(/* args */)
{
    m_SlotIndex = NbSlots++;
}

Slot::~Slot()
{
    NbSlots++;
}
