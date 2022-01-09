cmake_minimum_required(VERSION 3.16)
project(service)

set(CMAKE_CXX_STANDARD 20)

if (APPLE)
    message("Apple build")
    find_library(IOKIT_LIBRARY IOKit)
    find_library(CORE_LIBRARY CoreFoundation)
else ()
    message("Unix build")
    set(CORE_LIBRARY dl atomic)
endif ()

find_package(Boost REQUIRED COMPONENTS system filesystem thread)
find_package(Threads)

file(GLOB SRCS_G
        src/*.cpp
        src/joystick/*.cpp
        )


add_executable(${PROJECT_NAME} ${SRCS_G})

target_link_libraries(${PROJECT_NAME} ${CORE_LIBRARY} ${Boost_LIBRARIES} application wiringPi)
