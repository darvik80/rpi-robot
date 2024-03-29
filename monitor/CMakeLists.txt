cmake_minimum_required(VERSION 3.16)
project(monitor)


find_package(Boost 1.81 REQUIRED)
find_package(fmt)

file(GLOB SRCS_G src/*.cpp)

if (APPLE)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(CORE_LIBRARY CoreFoundation)
    file(COPY resources/settings.json DESTINATION ${PROPERTIES_DIR})

    file(GLOB SRCS_OS_G
            src/macos/smc.cpp
            )
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    file(COPY resources/settings.json DESTINATION ${PROPERTIES_DIR}/${PROJECT_NAME})
endif ()

set(RESOURCE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources)

if (APPLE)
    function(resource VAR SOURCE_PATH DESTINATION PATTERN)
        file(GLOB_RECURSE _LIST CONFIGURE_DEPENDS ${SOURCE_PATH}/${PATTERN})
        foreach (RESOURCE ${_LIST})
            get_filename_component(_PARENT ${RESOURCE} DIRECTORY)
            if (${_PARENT} STREQUAL ${SOURCE_PATH})
                set(_DESTINATION ${DESTINATION})
            else ()
                file(RELATIVE_PATH _DESTINATION ${SOURCE_PATH} ${_PARENT})
                set(_DESTINATION ${DESTINATION}/${_DESTINATION})
            endif ()
            set_property(SOURCE ${RESOURCE} PROPERTY MACOSX_PACKAGE_LOCATION ${_DESTINATION})
        endforeach (RESOURCE)
        set(${VAR} ${_LIST} PARENT_SCOPE)
    endfunction()

    resource(RESOURCES ${RESOURCE_DIRECTORY} Resources/ *.*)
    set(RESOURCES_G ${RESOURCES})

    add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SRCS_G} ${SRCS_OS_G} ${RESOURCES_G})
else ()
    file(COPY resources/settings.json DESTINATION {CMAKE_INSTALL_PREFIX}/etc/monitor_settings.json)
    add_executable(${PROJECT_NAME} ${SRCS_G})
endif ()
#file(COPY resources/server.pem DESTINATION ${PROPERTIES_DIR})

target_link_libraries(${PROJECT_NAME} ${IOKIT_LIBRARY} logging application iot-platform)
