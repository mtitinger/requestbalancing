#include "ModbusSlave.h"
#include <vector>
#include <getopt.h>

using namespace std;

int main(int argc, char **argv)
{
    uint8_t num_slaves = 1U;
    uint32_t tics_millisec = 100U;
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
            fprintf(stderr, "\t -d sim_duration_seconds (%u)\n", sim_duration_seconds);

            exit(EXIT_FAILURE);
        }
    }

    std::vector<ModbusSlave *> Slaves;

    for (uint8_t i = 0; i < num_slaves; i++)
    {
        ModbusSlave *slave = new ModbusSlave(tics_millisec, sim_duration_seconds);
        Slaves.push_back(slave);

        slave->Start();
    }

    for (auto it = Slaves.begin(); it != Slaves.end(); ++it)
    {
        (*it)->Join();
    }

    return 0;
}