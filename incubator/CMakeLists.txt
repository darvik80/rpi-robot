cmake_minimum_required(VERSION 3.16)
project(incubator)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SRCS_G
        src/*.cpp
        )

add_executable(${PROJECT_NAME} ${SRCS_G})

include_directories(
        /usr/include
        /usr/local/include
)

find_package(cJSON REQUIRED)
find_package(hidapi REQUIRED)

target_link_libraries(${PROJECT_NAME} logging core application hidapi::hidapi)
