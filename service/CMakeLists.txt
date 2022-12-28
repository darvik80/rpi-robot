cmake_minimum_required(VERSION 3.16)
project(service)

if (APPLE)
    message("Apple build")
    find_library(IOKIT_LIBRARY IOKit)
    find_library(CORE_LIBRARY CoreFoundation)
else ()
    message("Unix build")
endif ()

find_package(Boost REQUIRED COMPONENTS system filesystem thread)

if (CMAKE_HOST_SYSTEM_PROCESSOR MATCHES arm)
    set(RPI_LIB wiringPi)
elseif (CMAKE_HOST_SYSTEM_PROCESSOR MATCHES aarch64)
    set(RPI_LIB wiringPi)
endif ()

include_directories(
        /usr/local/include
)

file(GLOB SRCS_G
        src/*.cpp
        src/i2c/*.cpp
        )

file(COPY resources/settings.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(${PROJECT_NAME} ${SRCS_G})
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS} ${CORE_LIBRARY} ${Boost_LIBRARIES} ${RPI_LIB} application joystick)
