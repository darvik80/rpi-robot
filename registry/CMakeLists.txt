cmake_minimum_required(VERSION 3.16)
project(registry)

file(COPY resources/settings.json DESTINATION ${PROPERTIES_DIR})

file(GLOB SRCS_G src/*.cpp src/db/*.cpp)

find_package(libpqxx)

include_directories(
        ${libpqxx_INCLUDE_DIRS}
)

message(STATUS ${libpqxx_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SRCS_G} src/db/DataSource.cpp src/db/DataSource.h src/db/Database.cpp src/db/Database.h src/db/DatabaseProperties.h src/db/Repository.h src/db/DeviceRepository.cpp src/db/DeviceRepository.h src/db/SqlBuilder.h)
target_link_libraries(${PROJECT_NAME} logging application iot-platform libpqxx::pqxx)

install(TARGETS ${PROJECT_NAME})
