#include "Offset.h"


/* 10 slots in one second,
 * with 3req/slot in this sim => 15 requests = 50% cpu load, 50% of the slots*/
fake_mapping_t DEVICE_50PERCENT[18] =
    {
        /* slot*/
        {3500, string("REQ1"), 1},
        {3101, string("REQ2"), 1},
        {3102, string("REQ3"), 1},

        /* slot*/
        {3102, string("REQ4"), 1},
        {3104, string("REQ5"), 1},
        {3100, string("REQ6"), 1},

        /* slot*/
        {3101, string("REQ7"), 1},
        {3102, string("REQ8"), 1},
        {3103, string("REQ9"), 1},

        {3104, string("REQ10"), 1},
        {3105, string("REQ11"), 1},
        {3600, string("REQ12"), 1},

        {311, string("REQ13"), 1},
        {312, string("REQ14"), 1},
        {313, string("REQ15"), 1},

        {311, string("REQ16"), 1},
        {312, string("REQ17"), 1},
        {313, string("REQ18"), 1},
    };


fake_mapping_t FAKE_PTE1[21] =
    {
        {3500, string("MED"), 5},
        {3501, string("MED"), 5},
        {3502, string("MED"), 5},
        {3502, string("MED"), 5},
        {3504, string("MED"), 5},
        {3100, string("FAST"), 1},
        {3101, string("FAST"), 1},
        {3102, string("FAST"), 1},
        {3103, string("FAST"), 1},
        {3104, string("FAST"), 1},
        {3105, string("FAST"), 1},
        {310, string("FAST"), 60},
        {3601, string("SLOW"), 60},
        {3602, string("SLOW"), 60},
        {3603, string("SLOW"), 60},
        {4000, string("MIX"), 1},
        {4001, string("MIX"), 1},
        {4002, string("MIX"), 5},
        {4003, string("MIX"), 5},
        {4004, string("MIX"), 60},
        {4005, string("MIX"), 60}
    };



fake_mapping_t FAKE_PTE2[21] =
    {
        {3500, string("FAST"), 1},
        {3101, string("FAST"), 1},
        {3102, string("FAST"), 1},
        {3102, string("FAST"), 1},
        {3104, string("FAST"), 1},
        {3100, string("FST2"), 1},
        {3101, string("FST2"), 1},
        {3102, string("FST2"), 1},
        {3103, string("FST2"), 1},
        {3104, string("FST2"), 1},
        {3105, string("FST3"), 1},
        {3600, string("FST3"), 1},
        {311, string("FST3"), 1},
        {312, string("FST3"), 1},
        {313, string("FST3"), 1},
        {4000, string("FST3"), 1},
        {4001, string("FST3"), 1},
        {4002, string("FAST"), 1},
        {4003, string("FAST"), 1},
        {4004, string("FAST"), 1},
        {4001, string("FAST"), 1}
    };