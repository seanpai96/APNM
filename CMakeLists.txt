cmake_minimum_required(VERSION 3.22)
project(APNM)

set(CMAKE_CXX_STANDARD 11)

#include_directories(src/ara/com)
#include_directories(src/ara/core)
#include_directories(src/ara/nm)

add_executable(APNM src/main.cpp
        socket/socket.cpp
        src/ara/nm/CallbackTimer.cpp
        src/ara/nm/NetworkState_Handle0Impl.cpp
        src/ara/nm/NetworkState_Handle0Skeleton.cpp
        src/ara/nm/nm.cpp)
