cmake_minimum_required(VERSION 3.13)
project(krsri-rpi3)

message(STATUS "Architecture: ${CMAKE_HOST_SYSTEM_PROCESSOR}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv7-a+neon -mfpu=neon -O3")

# Dependencies
find_package(Threads REQUIRED)
find_library(wiringPi_LIBS NAMES wiringPi)
include_directories(external/sse2neon)

add_subdirectory(src)
