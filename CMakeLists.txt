cmake_minimum_required(VERSION 3.16)
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

add_subdirectory(core-utils)
add_subdirectory(service)
add_subdirectory(monitor)
