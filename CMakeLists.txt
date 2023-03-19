cmake_minimum_required(VERSION 3.16)

set(CMAKE_INSTALL_PREFIX /opt/iot)
project(rpi_robot)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif ()
if (APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
endif ()

set(Boost_NO_WARN_NEW_VERSIONS ON)
set(CONAN_DISABLE_CHECK_COMPILER ON)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(
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

add_subdirectory(monitor)
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
