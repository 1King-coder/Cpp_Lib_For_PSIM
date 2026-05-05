cmake_minimum_required(VERSION 4.1)
include_directories("C:/Altair/Altair_PSIM_2026/include")
include_directories("./PSIM_DLL/include")
include_directories("./PSIM_DLL/source")
project(C_FOR_SIM_LIB)

set(CMAKE_CXX_STANDARD 20)

add_library(C_FOR_SIM_LIB SHARED source/cpp_psim_lib.cpp
        include/Serial_utils.h
        source/Serial_utils.cpp)
