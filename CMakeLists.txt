cmake_minimum_required(VERSION 3.16)
set(CMAKE_INSTALL_PREFIX /opt/iot)
project(rpi_robot)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fvisibility=hidden")

set(CONAN_DISABLE_CHECK_COMPILER ON)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(
        core-utils/event-bus/src
        core-utils/logging/src
        core-utils/application/src
        core-utils/network/src
        core-utils/iot-platform/src
)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(PROPERTIES_DIR ${PROJECT_BINARY_DIR}/etc)
set(PLUGINS_DIR ${PROJECT_BINARY_DIR}/lib)
set(VAR_DIR ${PROJECT_BINARY_DIR}/var)

add_subdirectory(core-utils)

find_library(WIRING_PI wiringPi)
if(NOT WIRING_PI)
    message(STATUS "wiringPi library not found - can't build service app")
else()
    message(STATUS "wiringPi library found - add service app")
    add_subdirectory(service)
endif()
add_subdirectory(monitor)
