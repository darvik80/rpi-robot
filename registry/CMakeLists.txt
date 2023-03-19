cmake_minimum_required(VERSION 3.16)
project(registry)

file(COPY resources/settings.json DESTINATION ${PROPERTIES_DIR})

file(GLOB SRCS_G src/*.cpp)

add_executable(${PROJECT_NAME} ${SRCS_G})
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS} ${Boost_LIBRARIES} logging application iot-platform)

install(TARGETS ${PROJECT_NAME})
