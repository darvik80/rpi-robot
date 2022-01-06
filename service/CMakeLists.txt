cmake_minimum_required(VERSION 3.21)
project(service)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED COMPONENTS system filesystem thread)
find_package(Threads)

file(GLOB SRCS_G
        src/*.cpp
        )

link_directories(
        /usr/local/lib
)

add_executable(${PROJECT_NAME} ${SRCS_G})