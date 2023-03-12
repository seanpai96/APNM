cmake_minimum_required(VERSION 3.22)
project(APNM)

set(CMAKE_CXX_STANDARD 14)

#include_directories(src/ara/com)
#include_directories(src/ara/core)
#include_directories(src/ara/nm)

add_executable(APNM src/main.cpp)
