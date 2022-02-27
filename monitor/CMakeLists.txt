cmake_minimum_required(VERSION 3.16)
project(monitor)

if (APPLE)
    message("Apple build")
    find_library(IOKIT_LIBRARY IOKit)
    find_library(CORE_LIBRARY CoreFoundation)
    file(COPY resources/macos/settings.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

else ()
    message("Unix build")
    set(CORE_LIBRARY dl atomic)

    file(COPY resources/pi4/settings.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif ()

file(COPY resources/server.pem DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

find_package(Boost REQUIRED COMPONENTS system filesystem thread)
find_package(Threads)

file(GLOB SRCS_G
        src/*.cpp
        )

add_executable(${PROJECT_NAME} ${SRCS_G})
target_link_libraries(${PROJECT_NAME} ${IOKIT_LIBRARY} ${CORE_LIBRARY} ${Boost_LIBRARIES} application network)
