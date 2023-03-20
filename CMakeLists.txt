cmake_minimum_required(VERSION 3.16)

set(CMAKE_INSTALL_PREFIX /opt/iot)
project(rpi_robot)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

# libs {
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)
add_definitions(-DBOOST_STACKTRACE_GNU_SOURCE_NOT_REQUIRED -DBOOST_STACKTRACE_USE_ADDR2LINE)

find_package(Boost 1.81 REQUIRED)
find_package(fmt REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(nlohmann_json REQUIRED)
# }

include_directories(
        ${fmt_INCLUDE_DIRS}
        ${Boost_INCLUDE_DIRS}
        ${nlohmann_json_INCLUDE_DIRS}
        ${OpenSSL_INCLUDE_DIRS}

        core-utils/event-bus/src
        core-utils/logging/src
        core-utils/application/src
        core-utils/network/src
        core-utils/iot-platform/src
        core-utils/joystick/src
)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(PROPERTIES_DIR ${PROJECT_BINARY_DIR}/etc)
set(PLUGINS_DIR ${PROJECT_BINARY_DIR}/lib)
set(VAR_DIR ${PROJECT_BINARY_DIR}/var)

add_subdirectory(core-utils)

#add_subdirectory(monitor)
if (APPLE)
    message(STATUS "Apple build")
elseif (UNIX AND NOT APPLE)
    message(STATUS "Unix build")
    find_library(GPIOD gpiod)
    if (NOT GPIOD)
        message(STATUS "gpiod library not found - can't build service app")
    else ()
        message(STATUS "service with GPIO")
        add_subdirectory(service)
    endif ()
    message(STATUS "incubator with GPIO")
    add_subdirectory(incubator)
endif ()
add_subdirectory(registry)
