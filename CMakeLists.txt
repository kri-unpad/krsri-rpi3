cmake_minimum_required(VERSION 3.13)
project(krsri-rpi3)

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

# Applications
add_subdirectory(src)

# Packaging
set(CPACK_GENERATOR "TGZ")
set(CPACK_PACKAGE_FILE_NAME ${PROJECT_NAME}-${PROJECT_VERSION}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR})
include(CPack)
