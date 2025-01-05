cmake_minimum_required(VERSION 3.26)
project(OS_ex4)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(OS_ex4
        MemoryConstants.h
        PhysicalMemory.cpp
        PhysicalMemory.h
        Test.cpp
        VirtualMemory.cpp
        VirtualMemory.h
        TreeSearch.h
        TreeSearch.cpp)
