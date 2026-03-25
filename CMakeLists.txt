cmake_minimum_required(VERSION 4.1)
include_directories("C:/Altair/Altair_PSIM_2026/include")
project(C_FOR_SIM_LIB)

set(CMAKE_CXX_STANDARD 20)

add_library(C_FOR_SIM_LIB SHARED cpp_psim_lib.cpp)
