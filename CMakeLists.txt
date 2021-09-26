cmake_minimum_required(VERSION 3.1)

project (sched-sim)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -O1 -std=c++11")

add_executable(sched-sim main.cpp
    ModbusSlave.cpp
    Logger.cpp
    SystemMonitor.cpp
    Slot.cpp
    Offset.cpp
    )

target_link_libraries(sched-sim pthread)

