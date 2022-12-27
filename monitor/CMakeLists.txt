cmake_minimum_required(VERSION 3.16)
project(monitor)

if (APPLE)
    message("Apple build")
    find_library(IOKIT_LIBRARY IOKit)
    find_library(CORE_LIBRARY CoreFoundation)
    file(COPY resources/macos/settings.json DESTINATION ${PROPERTIES_DIR})
else ()
    message("Unix build " ${CMAKE_HOST_SYSTEM_PROCESSOR})

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

    if (CMAKE_HOST_SYSTEM_PROCESSOR MATCHES arm)
        file(COPY resources/orange/settings.json DESTINATION ${PROPERTIES_DIR})
    elseif (CMAKE_HOST_SYSTEM_PROCESSOR MATCHES aarch64)
        file(COPY resources/pi4/settings.json DESTINATION ${PROPERTIES_DIR})
    else()
        message(STATUS "No settings!!!")
    endif ()

endif ()
file(COPY resources/server.pem DESTINATION ${PROPERTIES_DIR})

find_package(Boost REQUIRED COMPONENTS system filesystem thread log log_setup REQUIRED)

file(GLOB SRCS_G src/*.cpp)

add_executable(${PROJECT_NAME} ${SRCS_G})
target_link_libraries(${PROJECT_NAME} ${IOKIT_LIBRARY} ${CONAN_LIBS} ${Boost_LIBRARIES} logging application iot-platform)

install(TARGETS ${PROJECT_NAME})

install(
        FILES ./resources/server.pem ./resources/pi4/settings.json
        DESTINATION ${CMAKE_INSTALL_PREFIX}/etc
)