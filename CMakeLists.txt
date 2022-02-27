cmake_minimum_required(VERSION 3.16)
project(rpi_robot)

set(CMAKE_CXX_STANDARD 17)

include_directories(
        /usr/local/opt/openssl/include
        core-utils/event-manager/src
        core-utils/logging/src
        core-utils/application/src
        core-utils/network/src
)

link_directories(
        /usr/local/lib
        /usr/local/Cellar/icu4c/69.1/lib
)

add_subdirectory(core-utils)
add_subdirectory(service)
add_subdirectory(monitor)
