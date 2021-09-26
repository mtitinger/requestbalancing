#include "ModbusSlave.h"

using namespace std;

int main(int argc, char** argv)
{

    ModbusSlave slave1;
    ModbusSlave slave2;

    slave1.Run();
    slave2.Run();

    return 0;
}