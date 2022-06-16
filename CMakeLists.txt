cmake_minimum_required(VERSION 3.16)
set(CMAKE_INSTALL_PREFIX /opt/iot)
project(rpi_robot)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fvisibility=hidden")

include_directories(
        /usr/local/include
        /usr/local/opt/openssl/include
        core-utils/event-manager/src
        core-utils/logging/src
        core-utils/application/src
        core-utils/network/src
)

link_directories(
        /usr/local/lib
        /usr/local/Cellar/icu4c/70.1/lib
)

set(Boost_USE_STATIC_LIBS       ON)  # only find static libs
set(Boost_USE_SHARED_LIBS       OFF)  # only find static libs

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
