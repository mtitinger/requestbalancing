#include "ModbusSlave.h"

using namespace std;

int seconds_sim = 60*10;


int main(int argc, char** argv)
{
    ModbusSlave slave1;
    ModbusSlave slave2;

    Logger logger_test("delme");
    logger_test.Test();

    slave1.Run(seconds_sim);
    slave2.Run(seconds_sim);

    return 0;
}