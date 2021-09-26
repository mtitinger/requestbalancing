#pragma once

#include <cstdint>
#include <string>
#include <algorithm>

using namespace std;

typedef struct _fm
{
    uint16_t value;
    string forumId;
    uint16_t refreshRate;
} fake_mapping_t;

extern fake_mapping_t FAKE_PTE1[21];
extern fake_mapping_t FAKE_PTE2[21];
extern fake_mapping_t DEVICE_50PERCENT[18];

class Offset
{
public:
    /* data */
    string m_forumId;
    uint16_t m_RefreshSeconds;
    uint16_t m_OffsetValue;

public:
    Offset(fake_mapping_t *mapping)
    {
        m_RefreshSeconds = mapping->refreshRate;
        m_forumId = mapping->forumId;
        m_OffsetValue = mapping->value;
    }

    ~Offset(){};

    string dump()
    {
        string ret = std::to_string(m_OffsetValue) + " in forumID '" + m_forumId + "' with T(s)=" + std::to_string(m_RefreshSeconds);

        return ret;
    };
};
