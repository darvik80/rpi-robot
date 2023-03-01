cmake_minimum_required(VERSION 3.16)
project(service)

if (APPLE)
    message("Apple build")
    find_library(IOKIT_LIBRAR Y IOKit)
    find_library(CORE_LIBRARY CoreFoundation)
endif ()

find_package(Boost REQUIRED COMPONENTS system filesystem thread)

if (CMAKE_HOST_SYSTEM_PROCESSOR MATCHES arm OR aarch64)
    set(RPI_LIB gpiod gpiodcxx)
endif ()

include_directories(
        /usr/local/include
)

file(GLOB SRCS_G
        src/*.cpp
        src/i2c/*.cpp
        src/gpiod/*.cpp
        )

file(COPY resources/settings.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(${PROJECT_NAME} ${SRCS_G})
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS} ${CORE_LIBRARY} ${Boost_LIBRARIES} ${RPI_LIB} application joystick)
