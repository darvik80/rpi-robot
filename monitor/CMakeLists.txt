cmake_minimum_required(VERSION 3.16)
project(monitor)

if (APPLE)
    message("Apple build")
    find_library(IOKIT_LIBRARY IOKit)
    find_library(CORE_LIBRARY CoreFoundation)
    file(COPY resources/macos/settings.json DESTINATION ${PROPERTIES_DIR})

else ()
    message("Unix build")
    set(CORE_LIBRARY dl atomic)

    if (CMAKE_HOST_SYSTEM_PROCESSOR MATCHES arm)
        file(COPY resources/orange/settings.json DESTINATION ${PROPERTIES_DIR})
    elseif (CMAKE_HOST_SYSTEM_PROCESSOR MATCHES aarch64)
        file(COPY resources/pi4/settings.json DESTINATION ${PROPERTIES_DIR})
    endif ()

endif ()
file(COPY resources/server.pem DESTINATION ${PROPERTIES_DIR})

find_package(Boost 1.74 REQUIRED COMPONENTS system filesystem thread)
find_package(Threads)

file(GLOB SRCS_G
        src/*.cpp
        )

add_executable(${PROJECT_NAME} ${SRCS_G})
target_link_libraries(${PROJECT_NAME} ${IOKIT_LIBRARY} ${CORE_LIBRARY} ${Boost_LIBRARIES} iot-platform)

install(TARGETS ${PROJECT_NAME})

install(
        FILES ./resources/server.pem ./resources/pi4/settings.json
        DESTINATION ${CMAKE_INSTALL_PREFIX}/etc
)