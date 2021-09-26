#include <vector>
#include <getopt.h>

#include "ModbusSlave.h"

using namespace std;

/* DEfault system parameter*/

#define HIGH_SPEED_DATA     1 /* seconds period*/
#define MEDIUM_SPEED_DATA   5 /* seconds period*/
#define LOW_SPEED_DATA      60 /* seconds period*/

#define REFRESH_TIC_DURATION        100U /* millisec */
#define AVERAGE_REQUEST_DURATION    30  /* millisec*/

int main(int argc, char **argv)
{
    uint8_t num_slaves = 1U;
    uint8_t average_requests_millisec = AVERAGE_REQUEST_DURATION;
    uint32_t tics_millisec = REFRESH_TIC_DURATION;
    uint32_t sim_duration_seconds = 10U;

    int opt;

    while ((opt = getopt(argc, argv, "s:t:d:")) != -1)
    {
        switch (opt)
        {
        case 's':
            num_slaves = strtoul(optarg, NULL, 0);
            break;
        case 't':
            tics_millisec = strtoul(optarg, NULL, 0);
            break;

        default:
            fprintf(stderr, "Usage: %s options:\n", argv[0]);
            fprintf(stderr, "\t -s num_slaves(%u)\n", num_slaves);
            fprintf(stderr, "\t -t tics_millisec(%u)\n", tics_millisec);
            fprintf(stderr, "\t -r average_requests_millisec(%u)\n", average_requests_millisec);
            fprintf(stderr, "\t -d sim_duration_seconds (%u)\n", sim_duration_seconds);

            exit(EXIT_FAILURE);
        }
    }

    uint8_t requests_per_slot = tics_millisec / average_requests_millisec;

    /* Instanciate a System Monitor*/
    SystemMonitor sysMon(   requests_per_slot,
                            HIGH_SPEED_DATA,
                            MEDIUM_SPEED_DATA,
                            LOW_SPEED_DATA);

    /* Instanciate MODBUS Slaves*/
    std::vector<ModbusSlave *> Slaves;

    for (uint8_t i = 0; i < num_slaves; i++)
    {
        ModbusSlave *slave = new ModbusSlave(tics_millisec, sim_duration_seconds, &sysMon);
        Slaves.push_back(slave);

        /* Setup offset to refresh for a "fake device" */
        for (uint16_t j = 0U; j < sizeof(DEVICE_50PERCENT)/sizeof(fake_mapping_t); j++ )
        {
            slave->AddOffsetToCache(&DEVICE_50PERCENT[j]);
        }


        slave->Start();
    }



    for (auto it = Slaves.begin(); it != Slaves.end(); ++it)
    {
        (*it)->Join();
    }

    return 0;
}