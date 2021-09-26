cmake_minimum_required(VERSION 3.1)

project (sched-sim)

add_executable(sched-sim main.cpp
    ModbusSlave.cpp
    )

target_link_libraries(sched-sim)

