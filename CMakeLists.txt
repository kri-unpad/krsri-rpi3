cmake_minimum_required(VERSION 3.13)
project(krsri-rpi3)

message(STATUS "Architecture: ${CMAKE_HOST_SYSTEM_PROCESSOR}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv8-a -mfloat-abi=hard -mfpu=neon-fp-armv8")

if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif()

# Dependencies
find_package(Threads REQUIRED)
find_library(wiringPi_LIBS NAMES wiringPi)
include_directories(external/sse2neon)

add_subdirectory(src)
